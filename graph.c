#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

//Allocates & initializes a graphlist with the specified graph g.
GraphList * makeGraphListSingleton(Graph * g);
//Computes correct byte and bit offsets within a row of our adjacency matrix.
void comp_indices(int j, int * byte, int * bit);

void comp_indices(int j, int * byte, int * bit) {
    *byte = j / 8;
    *bit = j % 8;
}


//Sets bit j within row i of matrix adj_mat to val.
void setBit(char ** adj_mat, int i, int j, char val) {
    int jbyte = 0, jbit = 0;
    comp_indices(j, &jbyte, &jbit);
    char * byte = adj_mat[i] + jbyte;
    char mask = true;
    if (val == true) {
        *byte = (mask << jbit) | *byte;
    }
    else {
        *byte = ~(mask << jbit) & *byte;
    }
}

//Returns value of bit i,j within matrix adj_mat.
char getBit(char ** adj_mat, int i, int j) {
    int jbyte = 0, jbit = 0;
    comp_indices(j, &jbyte, &jbit);
    char * byte = adj_mat[i] + jbyte;
    char mask = true;
    mask <<= jbit;
    return (mask & *byte) > 0;
}


//Allocates & initializes a basic terrain vg with num_vertices.
Graph * makeGraph(int num_vertices, int max_len) {
    Graph * g = malloc(sizeof(Graph));
    int i = 0;
    g->adj_mat = malloc(sizeof(char *) * max_len);

    for (i; i < max_len; i++) {
        //Compute number of columns to hold number of bits required for graph.
        int num_cols = (( (max_len - 1 ) / 8 ) + 1);
        g->adj_mat[i] = malloc(sizeof(char) * num_cols);
        memset(g->adj_mat[i], 0, sizeof(char) * num_cols);
    }

    g->len = num_vertices;
    g->max_len = max_len;
    
    //Initialize the visibilities. Nth vertex sees vertex n+1.
    for (i = 0; i < num_vertices - 1; i++) {
        setBit(g->adj_mat, i, i+1, true);
        setBit(g->adj_mat, i+1, i, true);
    }
    return g;
}

//Copies the graph g and returns the newly allocated copy.
Graph * graphCopy(Graph * g) {
    Graph * new = makeGraph(g->len, g->max_len);
    int i = 0;
    for (i = 0; i < g->len; i++) {
        int j = 0;
        for (j = 0; j < g->len; j++) {
            setBit(new->adj_mat, i, j, getBit(g->adj_mat, i, j));
        }
    }
    return new;
}


//Allocates & initializes an empty GraphList.
GraphList * makeGraphList() {
    GraphList * gl = malloc(sizeof(GraphList));
    memset(gl, 0, sizeof(GraphList));
    return gl;
}

//Allocates & initializes a graphlist with the specified graph g.
GraphList * makeGraphListSingleton(Graph * g) {
    GraphList * gl = makeGraphList();
    gl->g = g;
    return gl;
}

//Tacks a Graph g to the end of GraphList gl.
void append(Graph * g, GraphList * gl) {
    if (gl->g == NULL) {
        gl->g = g;
    }
    else if (gl->end == NULL){
        gl->next = makeGraphListSingleton(g);
        gl->next->prev = gl;
        gl->end = gl->next;
    }
    else {
        gl->end->next = makeGraphListSingleton(g);
        gl->end->next->prev = gl->end;
        gl->end = gl->end->next;
    }
}

//Concatenates GraphList b to GraphList a.
GraphList * concatenate (GraphList * a, GraphList * b) {
    GraphList * beginning = a;
    //Seek to the end of a.
    while(a->next != NULL)
        a = a->next;
    //Make sure we're at the beginning of b.
    while (b->prev != NULL)
        b = b->prev;
    a->next = b;
    return beginning;
}

void x_property(Graph * g) {
    //Can store the maximum possible number of x-property violating
    //vertices here.
    int * illegal_vertices = malloc(sizeof(int) * (g->len - 3));
    int illegal_ndx = 0;
    memset(illegal_vertices, 0, sizeof(int) * (g->len - 3));
    //Find the leftmost vertex that vertex n sees.
    int n = g->len - 1;
    int i = n - 1;
    int leftmost = -1;
    for (i; i > -1; i--) {
        if (getBit(g->adj_mat, n, i) == true) {
            leftmost = i;
        }
    }
    //Scan from right to left, beginning at the leftmost vertex-1, determining
    //all indices that break the x-property.
    for (i = leftmost - 1; i > -1; i--) {
        int j = 0;
        for (j = leftmost + 1; j < n; j++) {
            if (getBit(g->adj_mat, i, j) == true) { // (g->adj_mat[i][j] == true) {
                illegal_vertices[illegal_ndx] = i;
                illegal_ndx++;
                break;
            }
        }
    }
    //Fix order claim for each illegal vertex.
    for (i = 0; i < illegal_ndx; i++) {
        int ill_vert = illegal_vertices[i];
        setBit(g->adj_mat, ill_vert, n, true);
        setBit(g->adj_mat, n, ill_vert, true);
    }
    free(illegal_vertices);
}

//Determines if the graph is *still* broken after fixing order claim violations.
//Best to call this immediately after "x_property" (since fixing x-property
//can cause bar property violations.)
bool bar_property(Graph * g) {
    int i = 0;
    for (i; i < g->len; i++) {
        bool not_broken = false;
        int j = 0;
        for (j = i + 3; j < g->len; j++) {
            if (getBit(g->adj_mat, i, j) == true) {
                //If any of our vertices k don't break the property,
                //i and j are good.
                int k = 0;
                for (k = i + 1; k < j; k++) {
                    not_broken = not_broken || (getBit(g->adj_mat, i, k) && 
                                                getBit(g->adj_mat, j, k));
                }
                if (not_broken == false)
                    return false;
            }
        }
    }
    return true;
}

//Checks if graph g is a legal visibility graph by checking x-property and
//bar property. Makes the requisite changes if the graph can be fixed, and
//doesn't if it cannot.
bool fixGraph(Graph * g) {
    bool is_legal = false;
    x_property(g);
    is_legal = bar_property(g);
    return is_legal;
}

//Operations for deleting a single graph or a list of them.
void deleteGraph(Graph * g) {
    int i = 0;
    for (i; i < g->max_len; i++) {
        memset(g->adj_mat[i], 0, sizeof(char) * (( (g->max_len - 1) / 8 ) + 1));
        free(g->adj_mat[i]);
    }
    memset(g->adj_mat, 0, sizeof(char) * g->max_len);
    free(g->adj_mat);
    memset(g, 0, sizeof(Graph));
    free(g);
}

void deleteGraphs(GraphList * gl) {
    GraphList * curr = gl->end;
    for (curr; curr != NULL; curr = curr->prev) {
        deleteGraph(curr->g);
        curr->g = NULL;
        if (curr->next != NULL) {
            curr->next->prev = NULL;
            memset(curr->next, 0, sizeof(GraphList));
            free(curr->next);
            curr->next = NULL;
        }
    }
    gl->end = NULL;
}

//Writes a list of graphs to the file specified by filename. 
//Writes in append (w+) mode.
void writeGraphs(GraphList * gl, char * filename) {
    GraphList * curr = gl;
    FILE * file = fopen(filename, "w+");
    for (curr; curr != NULL; curr = curr->next) {
        Graph * g = curr->g;
        int i = 0;
        for (i; i < g->len; i++) {
            int j = 0;
            //Vertices don't see themselves, and each edge is bidirectional.
            for (j = i+1; j < g->len; j++) {
                if (getBit(g->adj_mat, i, j) == true) {
                    //Output format is v1 v2\n
                    fprintf(file, "%d %d\n", i, j);
                }
            }
        }
        fprintf(file, "---\n");
    }
    fclose(file);
}

void writeGraph(Graph * g, FILE * file) {
    int i = 0;
    for (i; i < g->len; i++) {
        int j = 0;
        //Vertices don't see themselves, and each edge is bidirectional.
        for (j = i+1; j < g->len; j++) {
            if (getBit(g->adj_mat, i, j) == true) {
                //Output format is v1 v2\n
                fprintf(file, "%d %d\n", i, j);
            }
        }
    }
    fprintf(file, "---\n");
}
