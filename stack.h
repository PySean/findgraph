//Defines the structure and operations for a stack.
#include <stdbool.h>

typedef struct stack {
    struct stack * prev;
    struct stack * next;
    //Signifies whether we connect an edge between corresponding 
    //vertex n and the final vertex
    bool connect;
    int n;
} Stack;

//Examines the value of the top of the stack.
bool top(Stack * s);

//Sets the value of the top of the stack.
void setTop(Stack * s, bool val);

//Pops a value off the stack
Stack * pop(Stack * s);

//Pushes a value onto the stack.
Stack * push(Stack * s, bool val);

//Outputs the content of the stack as a string of 1's and 0's.
void printStack(Stack * s);

//Returns the bottommost element of the stack.
Stack * bottom(Stack * s);
