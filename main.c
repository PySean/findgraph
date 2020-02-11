#include <stdio.h>
#include <stdlib.h>
#include "findgraph.h"


int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s: <number_of_vertices>\n", argv[0]);
        return 1;
    }
    findgraphs(atoi(argv[1]), "testfile");
    return 0;
}
