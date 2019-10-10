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
    struct graphlist * end;
} GraphList;

//Allocates & initializes a basic terrain vg with num_vertices.
Graph * makeGraph(int num_vertices);

//Allocates & initializes an empty GraphList.
GraphList * makeGraphList();

//Tacks a Graph g to the end of GraphList gl.
void append(Graph * g, GraphList * gl);

//Glues GraphList b to the end of GraphList a.
GraphList * concatenate (GraphList * a, GraphList * b);

//Generates the vg corresponding to stack s.
Graph * fromStack(Stack * s);

//Checks if graph g is a legal visibility graph by checking x property and
//bar property.
bool isLegal(Graph * g);

//"Transposes" graph b onto graph a. Meaning, all edges in b are
//made existent in a.
void transpose(Graph * a, Graph * b);

//Creates a set of new graphs by transposing previously computed graphs over
//the currently computed graph g.
GraphList * makeGraphs(Graph * g, GraphList * prevs);

//Operations for deleting a single graph or a list of them.
void deleteGraph(Graph * g);

void deleteGraphs(GraphList * gl);
