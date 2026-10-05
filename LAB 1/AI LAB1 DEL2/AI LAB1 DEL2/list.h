
#ifndef LIST_H
#define LIST_H

#define LIST_MAX 1000
#include "graph.h"

typedef struct {
    City* arr[LIST_MAX];
    int  cost[LIST_MAX];
    int  size;
} List;

void list_init(List* l);
int  list_contains(const List* l, const City* c);
void list_push(List* l, City* c, int distance);
City* list_pop_best(List* l, int* real_distance);
City* list_pop_best_f(List* l, int* real_distance);

#endif
