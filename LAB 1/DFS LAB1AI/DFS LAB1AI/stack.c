#include <stdlib.h>
#include "stack.h"

void stack_init(Stack* s)
{
	s->top = NULL;
};
int  stack_empty(Stack* s)
{
	return s->top == NULL;
};
void stack_push(Stack* s, Node v)
{
	SNode* newSNode = (SNode*)malloc(sizeof(SNode));
	if (newSNode == NULL)
	{
		printf("Error: Allocation\n");
		return;
	}
	newSNode->val = v;
	newSNode->next = s->top;
	s->top = newSNode;
};
Node stack_pop(Stack* s)
{
	if (stack_empty(s))
	{
		printf("Error: Stack is empty\n");
		return NULL;
	}
	SNode* temp = s->top;
	Node retVal = temp->val;
	s->top = s->top->next;
	free(temp);
	return retVal;
};