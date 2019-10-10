#include <stdbool.h>
#include <stdlib.h>
#include "graph.h"

//Allocates & initializes a basic terrain vg with num_vertices.
Graph * makeGraph(int num_vertices) {
    Graph * g = malloc(sizeof(Graph));
    int i = 0;
    g->adj_mat = malloc(sizeof(bool *) * num_vertices);

    for (i; i < num_vertices; i++)
        g->adj_mat[i] = malloc(sizeof(bool) * num_vertices);

    g->is_base = true;
    g->len = num_vertices;
    
    //Initialize the visibilities. Nth vertex sees vertex n+1.
    for (i = 0; i < num_vertices - 1; i++) {
        g->adj_mat[i][i+1] = true;
    }
    return g;
}

//Appends GraphList b to GraphList a.
GraphList * append (GraphList * a, GraphList * b) {
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

//Generates the vg corresponding to stack s.
//s should be pointing to the top of the stack so s->n is the number of
//vertices.
Graph * fromStack(Stack * s) {
    Graph * new = makeGraph(s->n);
    Stack * head = bottom(s);
    while (head != NULL) {
        if (head->connect == true) {
            new->adj_mat[head->n][s->n] = true;
            new->is_base = false;
        }
        head = head->next;
    }
    return new;
}

//Creates a set of new graphs by transposing previously computed graphs over
//the currently computed graph g.
GraphList * makeGraphs(Graph * g, GraphList * prevs);

//Checks if graph g is a legal visibility graph by checking order claim and
//bar property.
bool isLegal(Graph * g);

//Operations for deleting a single graph or a list of them.
void deleteGraph(Graph * g);

void deleteGraphs(GraphList * gl);
