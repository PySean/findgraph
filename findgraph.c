#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "findgraph.h"
//Since there's only two possibilities for graphs with 3 vertices, we begin
//our calculations with 4.
#define MIN_VERTICES 4
#define EXTEND 5
#define EXPLORE 6

/**
    "findgraph.c", by Sean Soderman
    Determines all possible visibility graphs.
*/
void graph_gen(Graph * prev, FILE * output, int ndx, bool checkVert, int option);
void findgraphs(int max_vertices, char * filename) {
    //First: Initialize an initial graphlist of graphs with n = 3.
    //GraphList * prevs = makeGraphList();
    Graph * init = makeGraph(3, max_vertices);
    FILE * outie = fopen(filename, "w+");
    graph_gen(init, outie, init->len - 3, false, EXPLORE);
    fclose(outie);
}

/**
    From a list of graphs with n-1 vertices in "prevs", generates a new
    list of graphs with n+1 vertices.

    "checkVert": true when we have added a new edge. False otherwise.
    "option": Set to either EXTEND or EXPLORE. The former makes a recursive call
    where nothing in the current graph is changed, however the number of vertices
    is increased by one. The latter makes a call "as usual": one that moves
    the vertex needle left and creates two scenarios.
*/
void graph_gen(Graph * prev, FILE * output, int ndx, bool checkVert, int option) {
    if (ndx < 0) {
        deleteGraph(prev);
        return;
    }
    //This *must* be done before the next if-block. The x_property function
    //operates from the final vertex leftwards, so extending the graph before
    //doing any fixes will mess with this.
    if (checkVert == true) {
        bool legal = fixGraph(prev);
        //No legal graph can be generated from an illegal graph, so we
        //stop here if we fail the bar property after fixing the x-property.
        if (legal == false) {
            deleteGraph(prev);
            return;
        }
    }
    if (option == EXTEND) {
        if (prev->len < prev->max_len) {
            prev->len++;
            setBit(prev->adj_mat, prev->len - 1, prev->len - 2, true);
            setBit(prev->adj_mat, prev->len - 2, prev->len - 1, true);
            ndx--; //Avoid redundant recursive calls.
        }
        else {
            writeGraph(prev, output);
            deleteGraph(prev);
            return;
        }
    }
    Graph * clean = NULL;
    //Copy of the modified graph we wish to extend.
    Graph * modEx = NULL;
    //Only recur to the left if we don't have a new edge to add for this spot.
    if (getBit(prev->adj_mat, ndx, prev->len - 1) != true) {

        clean = graphCopy(prev);
        //Extend the clean copy of the graph.
        Graph * cleanEx = graphCopy(prev);

        graph_gen(cleanEx, output, cleanEx->len - 1, false, EXTEND);

        setBit(prev->adj_mat, ndx, prev->len - 1, true);
        setBit(prev->adj_mat, prev->len - 1, ndx, true);
        modEx = graphCopy(prev);

        graph_gen(modEx, output, modEx->len - 1, true, EXTEND);
        graph_gen(prev, output, ndx - 1, true, EXPLORE);
    }
    if (clean != NULL) {
        graph_gen(clean, output, ndx - 1, false, EXPLORE);
    }
    else {
        graph_gen(prev, output, ndx - 1, false, EXPLORE);
    }
}
