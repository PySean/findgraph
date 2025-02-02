#include "stack.h"


typedef struct graph {
    //The adjacency matrix for the graph.
    bool ** adj_mat;
    //our matrices are always len x len (since they are adjacency matrices)
    int len;
    //The maximal length for a graph. Allows for extending the length of a graph
    //up to this length.
    int max_len;
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
Graph * makeGraph(int num_vertices, int max_len);

//Copies the graph g and returns the newly allocated copy.
Graph * graphCopy(Graph * g);

//Allocates & initializes an empty GraphList.
GraphList * makeGraphList();

//Tacks a Graph g to the end of GraphList gl.
void append(Graph * g, GraphList * gl);

//Glues GraphList b to the end of GraphList a.
GraphList * concatenate (GraphList * a, GraphList * b);

//Determines whether the x-property (order claim) holds for graph g,
//and fixes the errors if any arise.
void x_property(Graph * g);

//Determines if the graph is *still* broken after fixing order claim violations.
//Best to call this immediately after "x_property" (since fixing x-property
//can cause bar property violations.)
bool bar_property(Graph * g);

//Checks if graph g is a legal visibility graph by checking x property and
//bar property.
bool fixGraph(Graph * g);

//Operations for deleting a single graph or a list of them.
void deleteGraph(Graph * g);

void deleteGraphs(GraphList * gl);

//Writes a graph to the file specified by filename. Writes in append (w+) mode.
void writeGraphs(GraphList * gl, char * filename);
