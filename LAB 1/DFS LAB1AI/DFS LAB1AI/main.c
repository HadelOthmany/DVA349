#include "dfs_knapsack.h"
#include <stdio.h>
#include <time.h>

int main(void) {
    int MAXIMUMWEIGHT = 420;

    int items[][2] = {
        {20, 15}, {40, 32}, {50, 60}, {36, 80}, {26, 43},
        {64, 120}, {54, 77}, {18, 6}, {46, 93}, {28, 35}, {25, 37}
    };
    int nItems = (int)(sizeof(items) / sizeof(items[0]));
    clock_t start = clock();
    dfs_knapsack(items, nItems, MAXIMUMWEIGHT);
    clock_t end = clock();
    double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_used);
    return 0;
}
