#include "dfs_knapsack.h"
#include "node.h"
#include "stack.h"
#include <stdio.h>

void dfs_knapsack(int items[][2], int nItems, int MAXW) {
    Stack s;
    stack_init(&s);

    Node root = createNode(0, 0, 0, NULL, 0);
    stack_push(&s, root);

    int bestBenefit = 0;
    Node bestNode = root;

    while (!stack_empty(&s)) {
        Node cur = stack_pop(&s);

        if (cur->weight <= MAXW && cur->benefit > bestBenefit) {
            bestBenefit = cur->benefit;
            bestNode = cur;
        }

        if (cur->level == nItems) continue;

        int b = items[cur->level][0];
        int w = items[cur->level][1];
        int nextLevel = cur->level + 1;

        // Push INCLUDE first, so EXCLUDE is processed first (LIFO)
        if (cur->weight + w <= MAXW) {
            cur->right = createNode(nextLevel,
                cur->weight + w,
                cur->benefit + b,
                cur, 1);
            stack_push(&s, cur->right);
        }

        // Exclude always exists
        cur->left = createNode(nextLevel,
            cur->weight,
            cur->benefit,
            cur, 0);
        stack_push(&s, cur->left);
    }

    printf("Best benefit = %d\n", bestBenefit);
	printf("Total weight = %d\n", bestNode->weight);

    // Reconstruct chosen items
    printf("Items taken: ");
    int taken[15];
    int k = 0;

    Node p = bestNode;
    while (p && p->parent) {
        if (p->took == 1) {
            taken[k++] = p->parent->level; // item index chosen
        }
        p = p->parent;
    }

    for (int i = k - 1; i >= 0; --i) {
        printf("%d ", taken[i] + 1);
    }
    printf("\n");
}
