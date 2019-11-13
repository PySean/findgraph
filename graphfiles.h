//Definitions for structures and functions regarding all file-to-graph and
//graph-to-file manipulation.
#include <stdio.h>
#include "graph.h"

typedef struct fileblock {
    FILE * files;
    int len;
    int maxlen;
}FileBlock;

//Doubles the amount of memory fb is reserving for the files.
void extend(FileBlock * fb);

//Adds a new file object to the final element of the block.
//Going past the limit causes an extension.
void push(FileBlock * fb);

//Returns the final stored file object.
FILE pop();

//Frees up all the memory made for the file handles.
void deleteHandles();

//Flushes all graphs from the specified graphlist to a file.
//Deletes all graphs in the list afterwards.
void flushToFile(GraphList * gl);

//Recreates a GraphList from a file.
GraphList * fileToGraphList(FILE graphs);
