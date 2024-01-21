#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LEVEL 1000000000

struct ChitonsData {
    int width;
    int height;

    int **riskLevels;
};

struct Location {
    int x;
    int y;
    int risk;
};

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

struct LocationQueue {
    struct Location *data;
    int capacity;
    int size;
};

void swap(struct Location *a, struct Location *b) {
    struct Location temp = *b;

    *b = *a;
    *a = temp;
}   

void heapify(struct LocationQueue *queue, int index) {
    if (queue->size > 1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < queue->size && queue->data[left].risk < queue->data[smallest].risk) {
            smallest = left;
        }

        if (right < queue->size && queue->data[right].risk < queue->data[smallest].risk) {
            smallest = right;
        }

        if (smallest != index) {
            swap(&queue->data[index], &queue->data[smallest]);
            heapify(queue, smallest);
        }
    }
}

void enqueue(struct LocationQueue *queue, struct Location location) {
    if (queue->capacity == 0) {
        queue->capacity = 1000;
        queue->data = (struct Location *)malloc(queue->capacity * sizeof(struct Location));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 1000;
        queue->data = (struct Location *)realloc(queue->data, queue->capacity * sizeof(struct Location));
    }

    queue->data[queue->size++] = location;

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }
}

struct Location dequeue(struct LocationQueue *queue) {
    struct Location location = *queue->data;

    --queue->size;

    memcpy(queue->data, queue->data + 1, queue->size * sizeof(struct Location));

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }

    return location;
}

void clear(struct LocationQueue *queue) {
    while(queue->size) {
        dequeue(queue);
    }    
}

int lowestRisk(struct ChitonsData *data) {
    int **cumulativeRiskLevels = (int **)malloc(data->height * sizeof(int *));

    for (int y = 0; y < data->height; y++) {
        cumulativeRiskLevels[y] = (int *)calloc(data->width, sizeof(int));

        for (int x = 0; x < data->width; x++) {
            cumulativeRiskLevels[y][x] = MAX_LEVEL;
        }
    }

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct Location *));
    struct Location end = {data->width - 1, data->height - 1, 0};

    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    enqueue(queue, (struct Location){0, 0, 0});

    **cumulativeRiskLevels = 0;

    while (queue->size) {
        struct Location location = dequeue(queue);

        if (equal(location, end)) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];

            if (!(x >= 0 && x < data->width && y >= 0 && y < data->height)) {
                continue;
            }

            int risk = cumulativeRiskLevels[location.y][location.x] + data->riskLevels[y][x];

            if (cumulativeRiskLevels[y][x] > risk) {
                cumulativeRiskLevels[y][x] = risk;

                enqueue(queue, (struct Location){x, y, risk});
            }
        }
    }

    int risk = cumulativeRiskLevels[end.y][end.x];

    for (int y = 0; y < data->height; y++) {
        free(cumulativeRiskLevels[y]);
    }

    free(cumulativeRiskLevels);

    free(queue->data);
    free(queue);

    return risk;
}

struct ChitonsData *getChitonsData(char *input, int multiplier) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct ChitonsData *data = (struct ChitonsData *)calloc(1, sizeof(struct ChitonsData));

        char c = fgetc(inputFile);

        while (!feof(inputFile)) {
            if (c == '\n') {
                ++data->height;
            }

            if (!data->height) {
                ++data->width;
            }

            c = fgetc(inputFile);
        }

        ++data->height;

        fseek(inputFile, 0, SEEK_SET);

        data->riskLevels = (int **)calloc(data->height * multiplier, sizeof(int *));

        for (int y = 0; y < data->height * multiplier; y++) {
            data->riskLevels[y] = (int *)calloc(data->width * multiplier, sizeof(int));

            for (int x = 0; x < data->width * multiplier; x++) {
                if (x < data->width) {
                    data->riskLevels[y][x] = fgetc(inputFile) - '0';
                }
            }

            if (y < data->height) {
                fgetc(inputFile);
            }
        }

        fclose(inputFile);

        data->height *= multiplier;
        data->width *= multiplier;

        return data;
    }

    return NULL;
}

void freeChitonsData(struct ChitonsData *data) {
    for (int y = 0; y < data->height; y++) {
        free(data->riskLevels[y]);
    }

    free(data->riskLevels);
    free(data);
}