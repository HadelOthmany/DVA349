#define _CRT_SECURE_NO_WARNINGS
#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define TABLE_SIZE 211
static City* map[TABLE_SIZE];

static unsigned int hash(const char* s) {
    unsigned int h = 0;
    while (*s) h = (h * 31) + (unsigned char)(*s++);
    return h % TABLE_SIZE;
}

void graph_init(void) {
    for (int i = 0; i < TABLE_SIZE; i++)
        map[i] = NULL;
}

City* get_city(const char* name) {
    unsigned int idx = hash(name);
    City* cur = map[idx];

    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }

    City* c = (City*)malloc(sizeof(City));
    if (c == NULL)
        return NULL;

    strcpy(c->name, name);
    c->heuristic = -1;
    c->adj = NULL;
    c->next = map[idx];
    map[idx] = c;
    c->best_cost = INT_MAX;
    c->parent = NULL;

    return c;
}

void set_heuristic(const char* city, int h) {
    City* c = get_city(city);
    if (c) c->heuristic = h;
}

void add_edge(const char* a, const char* b, int cost) {
    City* A = get_city(a);
    City* B = get_city(b);
    if (!A || !B) return;

    Edge* fromA = (Edge*)malloc(sizeof(Edge));
    if (fromA == NULL)
        return NULL;
    strcpy(fromA->to, b);
    fromA->cost = cost;
    fromA->next = A->adj;
    A->adj = fromA;

    Edge* fromB = (Edge*)malloc(sizeof(Edge));
    if (fromB == NULL)
        return NULL;
    strcpy(fromB->to, a);
    fromB->cost = cost;
    fromB->next = B->adj;
    B->adj = fromB;
}

void graph_free(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        City* c = map[i];
        while (c) {
            Edge* e = c->adj;
            while (e) {
                Edge* enext = e->next;
                free(e);
                e = enext;
            }
            City* cnext = c->next;
            free(c);
            c = cnext;
        }
        map[i] = NULL;
    }
}

void graph_reset_search_state(void) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (City* c = map[i]; c; c = c->next) {
            c->best_cost = INT_MAX;
            c->parent = NULL;
        }
    }
}

