#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

//Returns the value of the top of the stack.
bool top(Stack * s) {
    return s->connect;
}

//Sets the value of the top of the stack.
void setTop(Stack * s, bool val) {
    s->connect = val;
}

//Pops a value off the stack
Stack * pop(Stack * s) {
    Stack * prev = s->prev;
    s->prev->next = NULL;
    s->prev = NULL;
    s->connect = false;
    s->n = 0;
    free(s);
    return prev;
}

//Pushes a value onto the stack.
Stack * push(Stack * s, bool val) {
    s->next = malloc(sizeof(Stack));
    memset(s->next, 0, sizeof(Stack));
    Stack * newHead = s->next;
    newHead->prev = s;
    newHead->next = NULL;
    newHead->connect = false;
    newHead->n = s->n + 1;
    newHead->connect = val;
    return newHead;
}

//Outputs the contents of the stack.
void printStack(Stack * s) {
    while (s->prev->n != 0) { //Ignore the dummy element at the top of the stack.
            s = s->prev;
    }
    while (s != NULL) {
        printf("%d ", s->connect);
        s = s->next;
    }
    printf("\n");
}

//Fetches the bottom element of the stack.
Stack * bottom(Stack *s) {
    while (s->prev->n != 0)
        s = s->prev;
    return s;
}
