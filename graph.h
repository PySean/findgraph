#include "stack.h"


typedef struct graph {
    //The adjacency matrix for the graph.
    bool ** adj_mat;
    //our matrices are always len x len
    int len;
    //Signifies whether we have edges other than the boundary of the vg.
    bool is_base; 
} Graph;

typedef struct graphlist {
    struct graphlist * prev;
    Graph * g;
    struct graphlist * next;
} GraphList;

//Allocates & initializes a basic terrain vg with num_vertices.
Graph * makeGraph(int num_vertices);

//Appends GraphList b to GraphList a.
GraphList * append (GraphList * a, GraphList * b);

//Generates the vg corresponding to stack s.
Graph * fromStack(Stack * s);

//Creates a set of new graphs by transposing previously computed graphs over
//the currently computed graph g.
GraphList * makeGraphs(Graph * g, GraphList * prevs);

//Checks if graph g is a legal visibility graph by checking order claim and
//bar property.
bool isLegal(Graph * g);

//Operations for deleting a single graph or a list of them.
void deleteGraph(Graph * g);

void deleteGraphs(GraphList * gl);
