#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stack.h"
#include "findgraph.h"

/**
    "findgraph.c", by Sean Soderman
    Brute forces visibility graphs that satisfy order claim and cordless 
    cycle.
*/
void findgraphs(int max_vertices) {
    int i = 0;
    int combination = 0; //This is a test to see if my stack algorithm is correct.
    int verts_to_check = max_vertices - 2; //Final two vertices aren't checked.
    for (i = 2; i <= verts_to_check; i++) {
        //We don't include the final two vertices, as no edges are drawn between
        //them and the final vertex.
        Stack * s = malloc(sizeof(Stack));
        memset(s, 0, sizeof(Stack));
        bool popped = false;
        do {
            //fprintf(stderr, "yolo!\n");
            if (popped) {
                if (top(s) == 0) {
                    setTop(s, true);
                    popped = false; //Go deeper down the stack again.
                }
                else {
                    s = pop(s);
                    popped = true;
                }
            }
            else {
                s = push(s, false);
                //fprintf(stderr, "In else clause.\n");
                if (s->n > i) {
                    //fprintf(stderr, "does this happen\n");
                    combination++;
                    s = pop(s);
                    popped = true;
                    printStack(s);
                    //Graph ** g = makeGraphs(s, prevGraphs);
                    //currGraphs.add(g);
                }
            }
        } while (s->n != 0);
        /*
        prevGraphs.delete();
        prevGraphs = currGraphs;
        writeToFile(currGraphs);
        */
        free(s);
    }
    printf("The number of combinations is %d\n", combination);
}
