#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "findgraph.h"
//Since there's only two possibilities for graphs with 3 vertices, we begin
//our calculations with 4.
#define MIN_VERTICES 4

/**
    "findgraph.c", by Sean Soderman
    Determines all possible visibility graphs.
*/
void findgraphs(int max_vertices, char * filename) {
    //First: Initialize an initial graphlist of graphs with n = 3.
    GraphList * prevs = makeGraphList();
    Graph * init = makeGraph(3, max_vertices);
    append(graphCopy(init), prevs);
    init->adj_mat[0][2] = true;
    init->adj_mat[2][0] = true;
    append(init, prevs);
    //Newly generated graphs.
    GraphList * news = makeGraphList();
    int i = 0;
    for (i = MIN_VERTICES; i <= max_vertices; i++) {
        Graph * prev = NULL;
        GraphList * curr = NULL;
        for (curr = prevs; curr != NULL; curr = prevs->next) {
            prev = curr->g;
            //Extend the previously computed graph by a vertex and edge.
            prev->adj_mat[prev->len][prev->len + 1] = true;
            prev->adj_mat[prev->len + 1][prev->len] = true;
            prev->len++;
            //We begin at the third vertex from the right, since we
            //only care about adding new edges to vertex n.
            graph_gen(prev, news, i - 2);
        }
        //NOTE: Might need a threshold for writing graphs if they occupy too much
        //memory.
        writeGraphs(news, filename);
        deleteGraphs(prevs);
        GraphList * temp = news;
        news = prevs;
        prevs = temp;
    }
}

/**
    From a list of graphs with n-1 vertices in "prevs", generates a new
    list of graphs with n+1 vertices.
*/
void graph_gen(Graph * prev, GraphList * news, int vert) {
    if (vert < 0)
        return;
    //First. Fix the graph, if possible. If not possible, we return.
    //Path 1: edge (vert, n) is picked, we check if it is OK.


}
