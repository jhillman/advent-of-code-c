#include <stdlib.h>
#include <stdbool.h>

struct Layer {
    int depth;
    int range;
    int severity;
};

struct Layers {
    int capacity;
    int count;
    struct Layer *data;
};


void addLayer(struct Layers *layers, int depth, int range) {
    if (layers->count == layers->capacity) {
        layers->capacity += 5;
        layers->data = (struct Layer *)realloc(layers->data, layers->capacity * sizeof(struct Layer));
    }

    struct Layer layer = { depth, range, depth * range };

    layers->data[layers->count++] = layer;
}

bool caught(struct Layer layer, int step) {
    return (step + layer.depth) % ((layer.range - 1) * 2) == 0;
}

void freeLayers(struct Layers *layers) {
    free(layers->data);
    free(layers);
}

struct Layers *getLayers(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Layers *layers = NULL;

    if (inputFile) {
        int depth;
        int range;
        int severity = 0;

        layers = (struct Layers *)calloc(1, sizeof(struct Layers));

        while (!feof(inputFile) && fscanf(inputFile, "%d: %d", &depth, &range)) {
            addLayer(layers, depth, range);
        }

        fclose(inputFile);
    }

    return layers;
}
