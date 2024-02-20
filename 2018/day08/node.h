#include "../../aoc.h"
#include <stdlib.h>

struct Node {
    int childNodeCount;
    int metadataEntryCount;

    struct Node **children;
    int *metadata;
};

struct Node *readNode(FILE *inputFile) {
    struct Node *node = (struct Node *)calloc(1, sizeof(struct Node));

    fscanf(inputFile, "%d %d", &node->childNodeCount, &node->metadataEntryCount);

    node->children = (struct Node **)malloc(node->childNodeCount * sizeof(struct Node *));

    for (int i = 0; i < node->childNodeCount; i++) {
        node->children[i] = readNode(inputFile);
    }

    node->metadata = (int *)malloc(node->metadataEntryCount * sizeof(int));

    for (int i = 0; i < node->metadataEntryCount; i++) {
        fscanf(inputFile, "%d", &node->metadata[i]);
    }

    return node;
}

int metadataSum(struct Node *node) {
    int sum = 0;

    for (int i = 0; i < node->metadataEntryCount; i++) {
        sum += node->metadata[i];
    }

    for (int i = 0; i < node->childNodeCount; i++) {
        sum += metadataSum(node->children[i]);
    }

    return sum;
}

int nodeValue(struct Node *node) {
    int value = 0;

    if (node->childNodeCount > 0) {
        for (int i = 0; i < node->metadataEntryCount; i++) {
            if (node->metadata[i] <= node->childNodeCount) {
                value += nodeValue(node->children[node->metadata[i] - 1]);
            }
        }
    } else {
        for (int i = 0; i < node->metadataEntryCount; i++) {
            value += node->metadata[i];
        }
    }

    return value;
}

void freeNode(struct Node *node) {
    for (int i = 0; i < node->childNodeCount; i++) {
        freeNode(node->children[i]);
    }

    if (node->children) {
        free(node->children);
    }

    if (node->metadata) {
        free(node->metadata);
    }

    free(node);
}