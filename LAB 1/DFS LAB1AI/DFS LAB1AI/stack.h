
#ifndef STACK_H

#define STACK_H

#include "node.h"

typedef struct SNode {
    Node val;
    struct SNode* next;
} SNode;

typedef struct {
    SNode* top;
} Stack;

void stack_init(Stack* s);
int  stack_empty(Stack* s);
void stack_push(Stack* s, Node v);
Node stack_pop (Stack* s);

#endif 