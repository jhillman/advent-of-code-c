#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct Circuit {
    int size;
    bool *boxes;
};

struct Circuits {
    int count;
    int capacity;
    struct Circuit **data;
};

void addCircuit(struct Circuits *circuits, struct Circuit *circuit) {
    if (circuits->count == circuits->capacity) {
        circuits->capacity += 100;
        circuits->data = (struct Circuit **)realloc(circuits->data, circuits->capacity * sizeof(struct Circuit *));
    }

    circuits->data[circuits->count++] = circuit;
}

int compareCircuits(const void *a, const void *b) {
    return (*((struct Circuit **)b))->size - (*((struct Circuit **)a))->size;
}

struct JunctionBox {
    int id;
    int x;
    int y;
    int z;
    struct Circuit *circuit;
};

struct JunctionBoxes {
    int count;
    int capacity;
    struct JunctionBox **data;
};

bool addBox(struct JunctionBoxes *boxes, struct JunctionBox *box) {
    if (boxes->count == boxes->capacity) {
        boxes->capacity += 100;
        boxes->data = (struct JunctionBox **)realloc(boxes->data, boxes->capacity * sizeof(struct JunctionBox *));
    }

    boxes->data[boxes->count++] = box;

    return true;
}

long distanceBetween(struct JunctionBox *box1, struct JunctionBox *box2) {
    long dx = box1->x - box2->x;
    long dy = box1->y - box2->y;
    long dz = box1->z - box2->z;
    
    return dx * dx + dy * dy + dz * dz;
}

struct Distance {
    struct JunctionBox *box1;
    struct JunctionBox *box2;
    long distance;
};

struct Distances {
    int count;
    int capacity;
    struct Distance *data;
};

void addDistance(struct Distances *distances, struct Distance distance) {
    if (distances->count == distances->capacity) {
        distances->capacity += 100;
        distances->data = (struct Distance *)realloc(distances->data, distances->capacity * sizeof(struct Distance));
    }

    distances->data[distances->count++] = distance;
}

int compareDistances(const void *a, const void *b) {
    long distance1 = ((struct Distance *)a)->distance;
    long distance2 = ((struct Distance *)b)->distance;

    if (distance1 < distance2) {
        return -1;
    } else if (distance1 > distance2) {
        return 1;
    } else {
        return 0;
    }
}

int createCircuits(char *input, bool complete) {
    FILE *inputFile = fopen(input, "r");
    int result = 0;

    if (inputFile) {
        struct JunctionBoxes *boxes = (struct JunctionBoxes *)calloc(1, sizeof(struct JunctionBoxes));
        struct Distances *distances = (struct Distances *)calloc(1, sizeof(struct Distances));
        int x;
        int y;
        int z;

        while (fscanf(inputFile, "%d,%d,%d", &x, &y, &z) == 3) {
            struct JunctionBox *box = (struct JunctionBox *)calloc(1, sizeof(struct JunctionBox));
            box->id = boxes->count;
            box->x = x;
            box->y = y;
            box->z = z;

            addBox(boxes, box);
        }

        fclose(inputFile);

        for (int i = 0; i < boxes->count; i++) {
            struct JunctionBox *box1 = boxes->data[i];

            for (int j = i + 1; j < boxes->count; j++) {
                struct JunctionBox *box2 = boxes->data[j];
                struct Distance distance = { box1, box2, distanceBetween(box1, box2) };

                addDistance(distances, distance);
            }
        }

        qsort(distances->data, distances->count, sizeof(struct Distance), compareDistances);

        struct Distance distance;
        struct Circuits *circuits = (struct Circuits *)calloc(1, sizeof(struct Circuits));
        
        int connections = 0;

        for (int i = 0; (complete || connections < 1000) && i < distances->count; i++) {
            distance = distances->data[i];
            struct Circuit *circuit;

            if (!distance.box1->circuit && !distance.box2->circuit) {
                circuit = (struct Circuit *)calloc(1, sizeof(struct Circuit));
                circuit->boxes = (bool *)calloc(boxes->count, sizeof(bool));
                circuit->boxes[distance.box1->id] = true;
                circuit->boxes[distance.box2->id] = true;
                circuit->size = 2;

                distance.box1->circuit = circuit;
                distance.box2->circuit = circuit;

                addCircuit(circuits, circuit);
                ++connections;
            } else if (distance.box1->circuit && !distance.box2->circuit) {
                distance.box1->circuit->boxes[distance.box2->id] = true;
                ++distance.box1->circuit->size;

                distance.box2->circuit = distance.box1->circuit;
                circuit = distance.box1->circuit;

                ++connections;
            } else if (distance.box2->circuit && !distance.box1->circuit) {
                distance.box2->circuit->boxes[distance.box1->id] = true;
                ++distance.box2->circuit->size;

                distance.box1->circuit = distance.box2->circuit;
                circuit = distance.box1->circuit;
                
                ++connections;
            } else {
                distance.box1->circuit->size = 0;

                for (int i = 0; i < boxes->count; i++) {
                    if (distance.box1->circuit->boxes[i] || distance.box2->circuit->boxes[i]) {
                        distance.box1->circuit->boxes[i] = true;
                        ++distance.box1->circuit->size;
                    }
                }

                if (distance.box1->circuit != distance.box2->circuit) {
                    struct Circuit *oldCircuit = distance.box2->circuit;

                    for (int i = 0; i < boxes->count; i++) {
                        if (boxes->data[i]->circuit == oldCircuit) {
                            boxes->data[i]->circuit = distance.box1->circuit;
                        }
                    }

                    oldCircuit->size = 0;
                }

                circuit = distance.box1->circuit;

                ++connections;
            }

            if (complete && circuit->size == boxes->count) {
                break;
            }
        }

        if (complete) {
            result = distance.box1->x * distance.box2->x;
        } else {
            qsort(circuits->data, circuits->count, sizeof(struct JunctionBoxes *), compareCircuits);

            for (int i = 0; i < 3; i++) {
                if (i == 0) {
                    result = circuits->data[i]->size;
                } else {
                    result *= circuits->data[i]->size;
                }
            }
        }

        for (int i = 0; i < circuits->count; i++) {
            free(circuits->data[i]->boxes);
            free(circuits->data[i]);
        }

        free(circuits);

        for (int i = 0; i < boxes->count; i++) {
            free(boxes->data[i]);
        }

        free(boxes->data);
        free(boxes);

        free(distances->data);
        free(distances);
    }

    return result;
}