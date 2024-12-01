#include <stdlib.h>
#include <stdbool.h>

struct Nodes;

struct Node {
    int id;
    struct Nodes *connections;
};

struct Nodes {
    int capacity;
    int count;
    struct Node **data;
};

struct Node *addNode(struct Nodes *nodes, struct Node *node) {
    if (nodes->count == nodes->capacity) {
        nodes->capacity += 5;
        nodes->data = (struct Node **)realloc(nodes->data, nodes->capacity * sizeof(struct Node *));
    }

    nodes->data[nodes->count++] = node;

    return node;
}

struct Node *getNode(struct Nodes *nodes, int id) {
    struct Node *node = nodes->data[id];

    if (!node) {
        node = (struct Node *)calloc(1, sizeof(struct Node));
        node->id = id;
        node->connections = (struct Nodes *)calloc(1, sizeof(struct Nodes));

        nodes->data[id] = node;
    }

    return node;
}

int connectedCount(struct Nodes *nodes, int id, bool *visited) {
    if (visited[id]) {
        return 0;
    }

    visited[id] = true;

    int count = 1;
    struct Node *node = nodes->data[id];

    if (node && node->connections) {
        for (int i = 0; i < node->connections->count; i++) {
            count += connectedCount(nodes, node->connections->data[i]->id, visited);
        }
    }

    return count;
}

void freeNodes(struct Nodes *nodes) {
    for (int i = 0; i < nodes->count; i++) {
        if (nodes->data && nodes->data[i]) {
            if (nodes->data[i]->connections) {
                if (nodes->data[i]->connections->data) {
                    free(nodes->data[i]->connections->data);
                }

                free(nodes->data[i]->connections);
            }

            free(nodes->data[i]);
        }
    }

    free(nodes->data);
    free(nodes);
}

struct Nodes *getNodes(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Nodes *nodes = NULL;

    if (inputFile) {
        nodes = (struct Nodes *)calloc(1, sizeof(struct Nodes));
        nodes->capacity = 2000;
        nodes->count = 2000;
        nodes->data = (struct Node **)calloc(2000, sizeof(struct Node *));

        int id;
        struct Node *node;
        char connections[64];
        char *connection;

        while (!feof(inputFile) && fscanf(inputFile, "%d <-> %[^\n]", &id, connections)) {
            node = getNode(nodes, id);

            connection = strtok(connections, ", ");

            while (connection) {
                addNode(node->connections, getNode(nodes, atoi(connection)));

                connection = strtok(NULL, ", ");
            }
        }

        fclose(inputFile);
    }

    return nodes;
}