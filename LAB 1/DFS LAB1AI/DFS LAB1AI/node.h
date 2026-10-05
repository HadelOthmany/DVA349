
#ifndef NODE_H
#define NODE_H

typedef struct treeNode {
    int level;                 // next item index to decide
    int weight;                // current weight
    int benefit;               // current benefit
    struct treeNode* left;     // exclude
    struct treeNode* right;    // include
    struct treeNode* parent;   // to reconstruct
    int took;                  // 0 exclude, 1 include
} treeNode;

typedef treeNode* Node;

Node createNode(int level, int weight, int benefit, Node parent, int took);

#endif
