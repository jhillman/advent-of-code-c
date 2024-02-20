#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct XYZ {
    int x;
    int y;
    int z;
};

int distanceBetween(struct XYZ first, struct XYZ second) {
    return abs(second.x - first.x) + abs(second.y - first.y) + abs(second.z - first.z);
}

struct Bot {
    struct XYZ location;
    int radius;
};

struct Bots {
    int capacity;
    int count;
    struct Bot *data;
};

void addBot(struct Bots *bots, struct Bot bot) {
    if (bots->count == bots->capacity) {
        bots->capacity += 100;
        bots->data = (struct Bot *)realloc(bots->data, bots->capacity * sizeof(struct Bot));
    }

    bots->data[bots->count++] = bot;
}

void freeBots(struct Bots *bots) {
    free(bots->data);
    free(bots);
}

struct Space {
    struct XYZ minimum;
    struct XYZ maximum;
    int distance;
    int area;
};

void calculateDetails(struct Space *space) {
    struct XYZ location = { 0, 0, 0 };
    int cornerDistance;

    space->distance = -1;

    struct XYZ corners[] = {
        { space->minimum.x, space->minimum.y, space->minimum.z },
        { space->maximum.x, space->minimum.y, space->minimum.z },
        { space->minimum.x, space->maximum.y, space->minimum.z },
        { space->maximum.x, space->maximum.y, space->minimum.z },
        { space->minimum.x, space->minimum.y, space->maximum.z },
        { space->maximum.x, space->minimum.y, space->maximum.z },
        { space->minimum.x, space->maximum.y, space->maximum.z },
        { space->maximum.x, space->maximum.y, space->maximum.z }
    };

    for (int i = 0; i < 8; i++) {
        cornerDistance = distanceBetween(location, corners[i]);

        if (space->distance == -1 || cornerDistance < space->distance) {
            space->distance = cornerDistance;
        }
    }

    space->area = (space->maximum.x - space->minimum.x + 1) * 
                  (space->maximum.y - space->minimum.y + 1) * 
                  (space->maximum.z - space->minimum.z + 1);
}

bool inRange(struct Bot bot, struct Space space) {
    return (
        max(space.minimum.x - bot.location.x, bot.location.x - space.maximum.x) +
        max(space.minimum.y - bot.location.y, bot.location.y - space.maximum.y) +
        max(space.minimum.z - bot.location.z, bot.location.z - space.maximum.z)
    ) <= bot.radius;
}

struct BotState {
    struct Bots *bots;
    struct Space space;
};

bool lessThan(struct BotState first, struct BotState second) {
    int result = second.bots->count - first.bots->count;

    if (result == 0) {
        result = second.space.distance - first.space.distance;
    }

    return result < 0;
}

struct BotStateQueue {
    int size;
    int capacity;
    struct BotState *data;
};

void swap(struct BotState *a, struct BotState *b) {
    struct BotState temp = *b;

    *b = *a;
    *a = temp;
}   

void heapify(struct BotStateQueue *queue, int index) {
    if (queue->size > 1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < queue->size && lessThan(queue->data[left], queue->data[smallest])) {
            smallest = left;
        }

        if (right < queue->size && lessThan(queue->data[right], queue->data[smallest])) {
            smallest = right;
        }

        if (smallest != index) {
            swap(&queue->data[index], &queue->data[smallest]);
            heapify(queue, smallest);
        }
    }
}

void enqueue(struct BotStateQueue *queue, struct BotState state) {
    if (queue->size == queue->capacity) {
        queue->capacity += 1000;
        queue->data = (struct BotState *)realloc(queue->data, queue->capacity * sizeof(struct BotState));
    }

    queue->data[queue->size++] = state;

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }
}

struct BotState dequeue(struct BotStateQueue *queue) {
    struct BotState state = *queue->data;

    --queue->size;

    memcpy(queue->data, queue->data + 1, queue->size * sizeof(struct BotState));

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }

    return state;
}

void clear(struct BotStateQueue *queue) {
    while(queue->size) {
        freeBots(dequeue(queue).bots);
    }    
}

void freeQueue(struct BotStateQueue *queue) {
    clear(queue);

    free(queue->data);
    free(queue);
}

struct Bots *getBots(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Bots *bots = NULL;

    if (inputFile) {
        struct Bot bot;

        bots = (struct Bots *)calloc(1, sizeof(struct Bots));

        while (fscanf(inputFile, "pos=<%d,%d,%d>, r=%d\n", &bot.location.x, &bot.location.y, &bot.location.z, &bot.radius) == 4) {
            addBot(bots, bot);
        }

        fclose(inputFile);
    }

    return bots;
}