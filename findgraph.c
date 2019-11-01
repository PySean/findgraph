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
void graph_gen(Graph * prev, GraphList * news, int vert, bool checkVert);
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
    GraphList * curr;
    for (i = MIN_VERTICES - 1; i < max_vertices - 1; i++) {
        Graph * prev = NULL;
        GraphList * curr = NULL;

        for (curr = prevs; curr != NULL; curr = curr->next) {
            prev = curr->g;
            //Extend the previously computed graph by a vertex and edge.
            prev->adj_mat[i][i+1] = true;
            prev->adj_mat[i+1][i] = true;
            prev->len++;
            //We begin at the third vertex from the right, since we
            //only care about adding new edges to vertex n.
            graph_gen(prev, news, prev->len - 3, false);
        }
        //NOTE: Might need a threshold for writing graphs if they occupy too much
        //memory.
        fprintf(stderr, "outside inner for loop.\n");
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

    "checkVert": true when we have added a new edge. False otherwise.
*/
void graph_gen(Graph * prev, GraphList * news, int vert, bool checkVert) {
    if (checkVert == true) {
        bool legal = fixGraph(prev);
        //No legal graph can be generated from an illegal graph, so we
        //stop here if we fail the bar property after fixing the x-property.
        if (legal == false)
            return;
    }
    if (vert < 0) {
        append(prev, news);
        return;
    }
    Graph * clean = graphCopy(prev);
    //Only recur to the left if we don't have a new edge to add for this spot.
    if (prev->adj_mat[vert][prev->len - 1] != true) {
        prev->adj_mat[vert][prev->len - 1] = true;
        graph_gen(prev, news, vert - 1, true);
    }
    graph_gen(clean, news, vert - 1, false);
}
