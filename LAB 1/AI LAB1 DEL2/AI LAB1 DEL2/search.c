#include "search.h"
#include "graph.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>


void greedy_best_first_search(const char* start, const char* goal) {
    City* start_city = get_city(start);
    City* goal_city = get_city(goal);

    List open, visited;
    list_init(&open);
    list_init(&visited);


    list_push(&open, start_city, 0);

    while (open.size > 0) {
        int cur_cost = 0;
        City* cur = list_pop_best(&open, &cur_cost);

        // avoid re-expanding
        if (list_contains(&visited, cur)) continue;
        list_push(&visited, cur, cur_cost);

        //printf("Expand: %s (h=%d)\n", cur->name, cur->heuristic);

        if (strcmp(cur->name, goal) == 0) {
            printf("Reached goal with greedy best-first search: %s\n", cur->name);

            printf("path:\n");
            for (int i = 0; i < visited.size; i++) {
                printf("  %s->", visited.arr[i]->name);
            }
            printf("\nTotal cost: %d\n", cur_cost);
            printf("\n\n\n ");

            return;
        }

        for (Edge* e = cur->adj; e; e = e->next) {
            City* neighbor = get_city(e->to);

            // Don't add nodes we've already expanded
            if (!list_contains(&visited, neighbor) && !list_contains(&open, neighbor)) {
                list_push(&open, neighbor, cur_cost + e->cost);
            }
        }
    }

    printf("No path found.\n");
}


void print_path(City* goal) {
    City* print_list[LIST_MAX];
    int n = 0;

    for (City* c = goal; c; c = c->parent)
        print_list[n++] = c;

    for (int i = n - 1; i >= 0; i--) {
        printf("%s", print_list[i]->name);
        if (i) printf(" -> ");
    }
    printf("\n");
}

void A_star_search(const char* start, const char* goal) {
    City* start_city = get_city(start);
    City* goal_city = get_city(goal);

    graph_reset_search_state();   // must reset best_cost + parent for all cities

    List open, closed;
    list_init(&open);
    list_init(&closed);

    // initialize start
    start_city->best_cost = 0;
    start_city->parent = NULL;

    list_push(&open, start_city, 0);   // store (city, g)

    while (open.size > 0) {
        int cur_cost;
        City* cur = list_pop_best_f(&open, &cur_cost);  // chooses min f = g + h

       

        // CLOSED membership check 
        if (list_contains(&closed, cur))
            continue;

        
        list_push(&closed, cur, cur_cost);

        // goal test
        if (cur == goal_city) {
            printf("Reached goal with A* search: %s\n", cur->name);
            printf("Path:\n");
            print_path(cur);
            printf("Total cost: %d\n", cur->best_cost);
            return;
        }

        // expand neighbors
        for (Edge* e = cur->adj; e; e = e->next) {
            City* neighbor = get_city(e->to);
            if (!neighbor) continue;

            if (list_contains(&closed, neighbor))
                continue;

            int new_cost = cur->best_cost + e->cost;   // g(neighbor)

            // Only accept improvement
            if (new_cost >= neighbor->best_cost)
                continue;

            neighbor->best_cost = new_cost;
            neighbor->parent = cur;

            // push new entry 
            list_push(&open, neighbor, new_cost);
        }
    }

    printf("No path found.\n");
}

