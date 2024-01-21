#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Node {
    char name[4];
    char leftName[4];
    char rightName[4];
    struct Node *left;
    struct Node *right;
};

struct Nodes {
    int capacity;
    int count;
    struct Node *data;
};

struct Map {
    struct Nodes *nodes;
    char *instructions;
};

void freeMap(struct Map *map) {
    free(map->nodes->data);
    free(map->instructions);
    free(map->nodes);
    free(map);
}

void addNode(struct Nodes *nodes, struct Node node) {
    if (nodes->count == nodes->capacity) {
        nodes->capacity += 50;
        nodes->data = (struct Node *)realloc(nodes->data, nodes->capacity * sizeof(struct Node));
    }

    nodes->data[nodes->count++] = node;
}

int compare(const void *a, const void *b) {
    return strcmp(((struct Node *)a)->name, ((struct Node *)b)->name);
}

struct Node *binarySearchNodes(struct Nodes *nodes, char name[4], int low, int high) {
    if (low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (strcmp(nodes->data[middle].name, name) == 0) {
        return &nodes->data[middle];
    }

    if (strcmp(nodes->data[middle].name, name) > 0) {
        return binarySearchNodes(nodes, name, low, middle - 1);
    } else {
        return binarySearchNodes(nodes, name, middle + 1, high);
    }
}

struct Map *getMap(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Map *map = NULL;

    if (inputFile) {
        char c;
        int instructionCount = 0;
        char name[4];
        char left[4];
        char right[4];
        struct Node node;

        do {
            ++instructionCount;

            c = fgetc(inputFile);
        } while (c != '\n');

        map = (struct Map *)calloc(1, sizeof(struct Map));
        map->nodes = (struct Nodes *)calloc(1, sizeof(struct Nodes));
        map->instructions = (char *)malloc(instructionCount);

        fseek(inputFile, 0, SEEK_SET);

        fgets(map->instructions, instructionCount, inputFile);
        map->instructions[strcspn(map->instructions, "\n")] = 0;

        fgetc(inputFile);
        fgetc(inputFile);

        while (fscanf(inputFile, "%3s = (%3s, %3s)\n", node.name, node.leftName, node.rightName) == 3) {
           addNode(map->nodes, node);
        }

        fclose(inputFile);

        qsort(map->nodes->data, map->nodes->count, sizeof(struct Node), compare);

        for (int i = 0; i < map->nodes->count; i++) {
            map->nodes->data[i].left = binarySearchNodes(map->nodes, map->nodes->data[i].leftName, 0, map->nodes->count);
            map->nodes->data[i].right = binarySearchNodes(map->nodes, map->nodes->data[i].rightName, 0, map->nodes->count);
        }
    }

    return map;
}

long getSteps(struct Map *map, struct Node *node, bool (*test)(struct Node *)) {
    char *instruction = map->instructions;
    long steps = 0;
    bool endFound = false;

    while (!test(node)) {
        node = *instruction == 'L' ? node->left : node->right;

        ++steps;
        ++instruction;

        if (*instruction == '\0') {
            instruction = map->instructions;
        }
    }

    return steps;
}