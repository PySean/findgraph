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
    //append(graphCopy(init), prevs);
    setBit(init->adj_mat, 0, 2, true);
    setBit(init->adj_mat, 2, 0, true);
    FILE * outie = fopen(filename, "w+");
    graph_gen(init, outie, init->len - 1, false, EXPLORE);
    fclose(outie);
    //append(init, prevs);
    //Newly generated graphs.
    //GraphList * news = makeGraphList();
    //int i = 0;
    //GraphList * curr;

}

/**
    From a list of graphs with n-1 vertices in "prevs", generates a new
    list of graphs with n+1 vertices.

    "checkVert": true when we have added a new edge. False otherwise.
    "option": Set to either EXTEND or EXPLORE. The former makes a recursive call
    where nothing in the current graph is changed, however the number of vertices
    is increased by one. The latter makes a call "as usual": one that moves
    the vertex needle left and creates two scenarios.
    ^^^ This might not be required, but it is nice to organise my thoughts.
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
            fprintf(stderr, "Baloney\n");
            deleteGraph(prev);
            return;
        }
    }
    if (option == EXTEND) {
        if (prev->len < prev->max_len) {
            prev->len++;
            setBit(prev->adj_mat, prev->len - 1, prev->len - 2, true);
            setBit(prev->adj_mat, prev->len - 2, prev->len - 1, true);
        }
        else {
            writeGraph(prev, output); //TODO: Need to create new "writeGraph" function.
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

        graph_gen(cleanEx, output, cleanEx->len, false, EXTEND);

        setBit(prev->adj_mat, ndx, prev->len - 1, true);
        setBit(prev->adj_mat, prev->len - 1, ndx, true);
        modEx = graphCopy(prev);

        graph_gen(prev, output, modEx->len, true, EXTEND);
        graph_gen(prev, output, ndx - 1, true, EXPLORE);
    }
    if (clean != NULL) {
        graph_gen(clean, output, ndx - 1, false, EXPLORE);
    }
    else {
        modEx = graphCopy(prev);
        graph_gen(prev, output, modEx->len, false, EXTEND);
        graph_gen(prev, output, ndx - 1, false, EXPLORE);
    }
}

/**
    Begin with graph of size n = 3. From right to left, i = verts - 2:
    if (len > maxlen):
        output(g)
        return
    
    for (i = g->len - 2; i >= 0; i--) {
        if final_ndx - i > 1: //Recur with vanilla graph first.
            addEdge(g, final_ndx, i)
            legal = fixgraph(g)
            NOTE: Probably want to clone in a seperate spot for more efficiency.
            if (legal == true):
                recur(clone(g)->len++) //Add edge here, extend by 1.
            else:
                removeEdge(g, final_ndx, i)
        else:
            recur(clone(g)->len++)
    }
    
    If legal, recurse to n + 1. If not, move left
    to next edge, then try to recurse.

    repeat until we hit n = maxverts. Then, output graph to stdout (or file).
*/
