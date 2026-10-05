#ifndef GRAPH_H
#define GRAPH_H

typedef struct Edge {
    char to[50];
	int cost; //g(n)
    struct Edge* next;
} Edge;

typedef struct City {
    char name[50];
    int heuristic;      // h(n)
    Edge* adj;          // neighbors
    struct City* next;  // hash chaining
    //A* fields 
    int best_cost;           // best known cost from start
    struct City* parent;  // for path reconstruction
} City;

void graph_init(void);
City* get_city(const char* name);
void set_heuristic(const char* city, int h);
void add_edge(const char* a, const char* b, int cost);
void graph_free(void);
void graph_reset_search_state(void);


#endif
