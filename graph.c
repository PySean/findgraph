#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

//Allocates & initializes a graphlist with the specified graph g.
GraphList * makeGraphListSingleton(Graph * g);

//Determines whether the x-property (order claim) holds for graph g,
//and fixes the errors if any arise.
void x_property(Graph * g);

//Determines if the graph is *still* broken after fixing order claim violations.
//Best to call this immediately after "x_property" (since fixing x-property
//can cause bar property violations.)
bool bar_property(Graph * g);

//Allocates & initializes a basic terrain vg with num_vertices.
Graph * makeGraph(int num_vertices, int max_len) {
    Graph * g = malloc(sizeof(Graph));
    int i = 0;
    g->adj_mat = malloc(sizeof(bool *) * max_len);

    for (i; i < num_vertices; i++) {
        g->adj_mat[i] = malloc(sizeof(bool) * max_len);
        memset(g->adj_mat[i], '0', sizeof(bool) * max_len);
    }

    g->is_base = true;
    g->len = num_vertices;
    g->max_len = max_len;
    
    //Initialize the visibilities. Nth vertex sees vertex n+1.
    for (i = 0; i < num_vertices - 1; i++) {
        g->adj_mat[i][i+1] = true;
        g->adj_mat[i+1][i] = true;
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
            new->adj_mat[i][j] = g->adj_mat[i][j];
        }
    }
    return new;
}


//Allocates & initializes an empty GraphList.
GraphList * makeGraphList() {
    GraphList * gl = malloc(sizeof(GraphList));
    memset(gl, '0', sizeof(GraphList));
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
    memset(illegal_vertices, '0', g->len);
    //Find the leftmost vertex that vertex n sees.
    int n = g->len;
    int i = n - 1;
    int leftmost = -1;
    for (i; i > 0; i--) {
        if (g->adj_mat[n][i] == true)
            leftmost = i;
    }
    //Scan from right to left, beginning at the leftmost vertex-1, determining
    //all indices that break the x-property.
    for (i = leftmost - 1; i > 0; i--) {
        int j = 0;
        for (j = leftmost + 1; j < n; j++) {
            if (g->adj_mat[i][j] == true) {
                illegal_vertices[illegal_ndx] = i;
                illegal_ndx++;
                break;
            }
        }
    }
    //Fix order claim for each illegal vertex.
    for (i = 0; i < illegal_ndx; i++) {
        g->adj_mat[i][n] = true;
        g->adj_mat[n][i] = true;
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
            if (g->adj_mat[i][j] == true) {
                //If any of our vertices k don't break the property, 
                //i and j are good.
                int k = 0;
                for (k = i + 1; k < j; k++) {
                    not_broken = not_broken || (g->adj_mat[i][k] && g->adj_mat[j][k]);
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
void deleteGraph(Graph * g);

void deleteGraphs(GraphList * gl);
