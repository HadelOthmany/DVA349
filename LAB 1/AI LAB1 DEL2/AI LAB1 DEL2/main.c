#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "search.h"


int main(void) {
    graph_init();


    FILE* fptr = fopen("Spain_map.txt", "r");
    if (!fptr) { perror("fopen"); return 1; }

    char line[256];
    char A[100], B[100];
    int distance;

    while (fgets(line, sizeof(line), fptr)) {
        // Try 3 tokens first: "A B 123"
        if (sscanf(line, "%99s %99s %d", A, B, &distance) == 3) {
            add_edge(A, B, distance);
        }
        // Then 2 tokens: "A 123"  (heuristic line)
        else if (sscanf(line, "%99s %d", A, &distance) == 2) {
            set_heuristic(A, distance);
        }
    }
    fclose(fptr);

    greedy_best_first_search("Malaga", "Valladolid");
    A_star_search("Malaga", "Valladolid");

    graph_free();
    return 0;
}
