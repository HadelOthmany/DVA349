#include <stdlib.h>
#include "list.h"

void list_init(List* l) {
    l->size = 0;
}

int list_contains(const List* l, const City* c) {
    for (int i = 0; i < l->size; i++)
        if (l->arr[i] == c) return 1;
    return 0;
}

void list_push(List* l, City* c, int distance)
{
    if (l->size >= LIST_MAX) return;
    l->arr[l->size] = c;
    l->cost[l->size] = distance;
    l->size++;
}

City* list_pop_best(List* l, int* real_distance) {
    if (l->size == 0) return NULL;

    int best_i = 0;
    int best_h = l->arr[0]->heuristic;

    for (int i = 1; i < l->size; i++) {
        int h = l->arr[i]->heuristic;
        if (h < best_h) {
            best_h = h;
            best_i = i;
        }
    }

    City* best = l->arr[best_i];
    *real_distance = l->cost[best_i];

    l->arr[best_i] = l->arr[l->size - 1];
    l->cost[best_i] = l->cost[l->size - 1];
    l->size--;

    return best;
}

City* list_pop_best_f(List* l, int* real_distance) {
    if (l->size == 0) return NULL;

    int best_i = 0;

    int best_g = l->cost[0];
    int best_h = l->arr[0]->heuristic;
    int best_f = best_g + best_h;

    for (int i = 1; i < l->size; i++) {
        int g = l->cost[i];
        int h = l->arr[i]->heuristic;
        int f = g + h;

        // Choose smallest f
        if (f < best_f) {
            best_i = i;
            best_g = g;
            best_h = h;
            best_f = f;
        }
    }

    City* best = l->arr[best_i];
    *real_distance = l->cost[best_i];

    // remove by swap-with-last
    l->arr[best_i] = l->arr[l->size - 1];
    l->cost[best_i] = l->cost[l->size - 1];
    l->size--;

    return best;
}