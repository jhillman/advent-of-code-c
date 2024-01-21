#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#define HALLWAY_LENGTH 11

struct Room {
    int hallwayPosition;
    char amphipod;
    char amphipods[ROOM_DEPTH];
};

struct Burrow {
    char hallway[HALLWAY_LENGTH];
    struct Room aRoom;
    struct Room bRoom;
    struct Room cRoom;
    struct Room dRoom;
};

void printBurrow(struct Burrow *burrow) {
    printf("#############\n");

    printf("#");
    for (int i = 0; i < HALLWAY_LENGTH; i++) {
        printf("%c", burrow->hallway[i]);
    }
    printf("#\n");

    for (int i = 0; i < ROOM_DEPTH; i++) {
        printf("%s#%c#%c#%c#%c#%s\n", i == 0 ? "##" : "  ", 
            burrow->aRoom.amphipods[i], 
            burrow->bRoom.amphipods[i], 
            burrow->cRoom.amphipods[i], 
            burrow->dRoom.amphipods[i], i == 0 ? "##" : "  ");
    }

    printf("  #########  \n\n");
}

struct Room *roomForAmphipod(struct Burrow *burrow, char amphipod) {
    struct Room *room;

    switch (amphipod) {
        case 'A':
            room = &burrow->aRoom;
            break;
        case 'B':
            room = &burrow->bRoom;
            break;
        case 'C':
            room = &burrow->cRoom;
            break;
        case 'D':
            room = &burrow->dRoom;
            break;
    }

    return room;
}

bool burrowsEqual(struct Burrow *first, struct Burrow *second) {
    int result = result = strncmp(first->hallway, second->hallway, HALLWAY_LENGTH);

    if (result == 0) {
        result = strncmp(first->aRoom.amphipods, second->aRoom.amphipods, ROOM_DEPTH);
    }

    if (result == 0) {
        result = strncmp(first->bRoom.amphipods, second->bRoom.amphipods, ROOM_DEPTH);
    }

    if (result == 0) {
        result = strncmp(first->cRoom.amphipods, second->cRoom.amphipods, ROOM_DEPTH);
    }

    if (result == 0) {
        result = strncmp(first->dRoom.amphipods, second->dRoom.amphipods, ROOM_DEPTH);
    }

    return result == 0;
}

unsigned int burrowHash(struct Burrow *burrow) {
    unsigned int hash = 0;

    for (int i = 0; i < HALLWAY_LENGTH; i++) {
        hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + (unsigned int)burrow->hallway[i];
    }

    for (int i = 0; i < ROOM_DEPTH; i++) {
        hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + (unsigned int)burrow->aRoom.amphipods[i];
    }

    for (int i = 0; i < ROOM_DEPTH; i++) {
        hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + (unsigned int)burrow->bRoom.amphipods[i];
    }

    for (int i = 0; i < ROOM_DEPTH; i++) {
        hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + (unsigned int)burrow->cRoom.amphipods[i];
    }

    for (int i = 0; i < ROOM_DEPTH; i++) {
        hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + (unsigned int)burrow->dRoom.amphipods[i];
    }

    return hash;
}

struct BurrowState {
    struct Burrow burrow;
    int energy;
};

void swap(struct BurrowState **a, struct BurrowState **b) {
    struct BurrowState *temp = *b;

    *b = *a;
    *a = temp;
}   

struct BurrowStateQueue {
    struct BurrowState **data;
    int capacity;
    int size;
};

void heapify(struct BurrowStateQueue *queue, int index) {
    if (queue->size > 1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < queue->size && queue->data[left]->energy < queue->data[smallest]->energy) {
            smallest = left;
        }

        if (right < queue->size && queue->data[right]->energy < queue->data[smallest]->energy) {
            smallest = right;
        }

        if (smallest != index) {
            swap(&queue->data[index], &queue->data[smallest]);
            heapify(queue, smallest);
        }
    }
}

void enqueueBurrowState(struct BurrowStateQueue *queue, struct BurrowState *burrowState) {
    if (queue->capacity == 0) {
        queue->capacity = 10000;
        queue->data = (struct BurrowState **)malloc(queue->capacity * sizeof(struct BurrowState *));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 10000;
        queue->data = (struct BurrowState **)realloc(queue->data, queue->capacity * sizeof(struct BurrowState *));
    }

    queue->data[queue->size++] = burrowState;

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }
}

struct BurrowState *dequeueBurrowState(struct BurrowStateQueue *queue) {
    struct BurrowState *burrowState = *queue->data;

    --queue->size;

    memcpy(queue->data, queue->data + 1, queue->size * sizeof(struct BurrowState *));

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }

    return burrowState;
}

bool roomAvailable(struct Room *room) {
    bool available = *room->amphipods == '.';

    for (int i = 1; available && i < ROOM_DEPTH; i++) {
        available = room->amphipods[i] == '.' || room->amphipods[i] == room->amphipod;
    }

    return available;
}

bool roomHasAmphipodsToMove(struct Room *room) {
    for (int i = 0; i < ROOM_DEPTH; i++) {
        if (isalpha(room->amphipods[i]) && room->amphipods[i] != room->amphipod) {
            return true;
        }
    }

    return false;
}

bool burrowHallwayPathClear(char *hallway, int start, int end) {
    bool clear = true;
    char *space = hallway + start;

    if (start < end) {
        while (clear && ++space <= hallway + end) {
            clear = *space == '.';
        }
    } else {
        while (clear && --space >= hallway + end) {
            clear = *space == '.';
        }
    }

    return clear;
}

int organizeBurrow(struct Burrow original) {
    struct BurrowStateQueue *burrowStateQueue = (struct BurrowStateQueue *)calloc(1, sizeof(struct BurrowStateQueue *));
    struct Burrow organized = {
        { "..........." },
        { 2, 'A', "" },
        { 4, 'B', "" },
        { 6, 'C', "" },
        { 8, 'D', "" },
    };

    for (int i = 0; i < ROOM_DEPTH; i++) {
        organized.aRoom.amphipods[i] = 'A';
        organized.bRoom.amphipods[i] = 'B';
        organized.cRoom.amphipods[i] = 'C';
        organized.dRoom.amphipods[i] = 'D';
    }

    char amphipods[] = "ABCD";
    int amphipodEnergy[] = { 1, 10, 100, 1000 };

    int *burrowStateCosts = (int *)calloc(5000000000, sizeof(int));

    int energy = 0;

    struct BurrowState *burrowState = (struct BurrowState *)calloc(1, sizeof(struct BurrowState));
    burrowState->burrow = original;
    burrowState->energy = 0;

    enqueueBurrowState(burrowStateQueue, burrowState);

    int printCount = 0;

    while (burrowStateQueue->size) {
        struct BurrowState *burrowState = dequeueBurrowState(burrowStateQueue);

        struct Burrow *burrow = &burrowState->burrow;

        if (burrowsEqual(burrow, &organized)) {
            energy = burrowState->energy;

            free(burrowState);

            break;
        }

        // move amphipods in the hallway into their rooms
        for (int i = 0; i < HALLWAY_LENGTH; i++) {
            if (isalpha(burrow->hallway[i])) {
                char amphipod = burrow->hallway[i];
                struct Room *room = roomForAmphipod(burrow, amphipod);

                if (roomAvailable(room) && burrowHallwayPathClear(burrow->hallway, i, room->hallwayPosition)) {
                    int roomPosition = 0;

                    while (room->amphipods[roomPosition + 1] == '.' && roomPosition < ROOM_DEPTH - 1) {
                        ++roomPosition;
                    }

                    int stepsIntoRoom = roomPosition + 1;
                    int energy = burrowState->energy + 
                        (abs(i - room->hallwayPosition) + stepsIntoRoom) * amphipodEnergy[amphipod - 'A'];

                    struct Burrow newBurrow = *burrow;
                    newBurrow.hallway[i] = '.';

                    roomForAmphipod(&newBurrow, room->amphipod)->amphipods[roomPosition] = room->amphipod;

                    unsigned int hash = burrowHash(&newBurrow);

                    if (burrowStateCosts[hash] == 0 || energy < burrowStateCosts[hash]) {
                        struct BurrowState *newBurrowState = (struct BurrowState *)calloc(1, sizeof(struct BurrowState));
                        newBurrowState->burrow = newBurrow;
                        newBurrowState->energy = energy;

                        burrowStateCosts[hash] = energy;

                        enqueueBurrowState(burrowStateQueue, newBurrowState);
                    }
                }
            }
        }

        // move amphipods in wrong rooms into correct rooms or into the hallway
        for (int i = 0; i < 4; i++) {
            struct Room *room = roomForAmphipod(burrow, amphipods[i]);

            if (roomHasAmphipodsToMove(room)) {
                char *roomPosition = room->amphipods;

                while (*roomPosition == '.') {
                    ++roomPosition;
                }

                int amphipodPosition = roomPosition - room->amphipods;
                char amphipod = *roomPosition;
                int stepsOutOfRoom = amphipodPosition + 1;

                struct Room *destinationRoom = roomForAmphipod(burrow, amphipod);

                if (roomAvailable(destinationRoom) && burrowHallwayPathClear(burrow->hallway, room->hallwayPosition, destinationRoom->hallwayPosition)) {
                    struct Burrow newBurrow = *burrow;

                    int roomPosition = 0;

                    while (destinationRoom->amphipods[roomPosition + 1] == '.' && roomPosition < ROOM_DEPTH - 1) {
                        ++roomPosition;
                    }

                    int stepsIntoRoom = roomPosition + 1;

                    roomForAmphipod(&newBurrow, room->amphipod)->amphipods[amphipodPosition] = '.';
                    roomForAmphipod(&newBurrow, destinationRoom->amphipod)->amphipods[roomPosition] = destinationRoom->amphipod;

                    int energy = burrowState->energy + 
                        (stepsOutOfRoom + abs(room->hallwayPosition - destinationRoom->hallwayPosition) + stepsIntoRoom) * amphipodEnergy[amphipod - 'A'];

                    unsigned int hash = burrowHash(&newBurrow);

                    if (burrowStateCosts[hash] == 0 || energy < burrowStateCosts[hash]) {
                        struct BurrowState *newBurrowState = (struct BurrowState *)calloc(1, sizeof(struct BurrowState));
                        newBurrowState->burrow = newBurrow;
                        newBurrowState->energy = energy;

                        burrowStateCosts[hash] = energy;

                        enqueueBurrowState(burrowStateQueue, newBurrowState);
                    }
                } else {
                    for (int j = 0; j < HALLWAY_LENGTH; j++) {
                        if ((j == 0 || j == 10 || j % 2 == 1) && burrow->hallway[j] == '.' && burrowHallwayPathClear(burrow->hallway, j, room->hallwayPosition)) {
                            struct Burrow newBurrow = *burrow;
                            newBurrow.hallway[j] = amphipod;

                            int movingAmphipodHallwayPosition = roomForAmphipod(&newBurrow, amphipod)->hallwayPosition;

                            roomForAmphipod(&newBurrow, room->amphipod)->amphipods[amphipodPosition] = '.';

                            bool hallwayPositionValid = true;

                            // see if the recently moved amphipod and another amphipod are both blocking one another from their rooms
                            for (int k = 0; hallwayPositionValid && k < HALLWAY_LENGTH; k++) {
                                if (k != j && isalpha(newBurrow.hallway[k]) && newBurrow.hallway[k] != amphipod) {
                                    char hallwayAmphipod = newBurrow.hallway[k];
                                    int hallwayAmphipodHallwayPosition = roomForAmphipod(&newBurrow, hallwayAmphipod)->hallwayPosition;
                                    int minHallwayPosition = min(movingAmphipodHallwayPosition, hallwayAmphipodHallwayPosition);
                                    int maxHallwayPosition = max(movingAmphipodHallwayPosition, hallwayAmphipodHallwayPosition);

                                    if (j > minHallwayPosition && j < maxHallwayPosition && 
                                        k > minHallwayPosition && k < maxHallwayPosition) {
                                        hallwayPositionValid = false;
                                    }
                                }
                            }

                            // make sure there is enough space in the hallway to clear a room for an amphipod waiting in the hallway
                            // only check the end rooms for part 2
                            if (hallwayPositionValid && ROOM_DEPTH == 2 || (amphipod == 'A' && (j == 3 || j == 5)) || (amphipod == 'D' && (j == 5 || j == 7))) {
                                struct Room *destinationRoom = roomForAmphipod(burrow, amphipod);
                                char *roomPosition = destinationRoom->amphipods + ROOM_DEPTH - 1;
                                int amphipodsToMove = 0;

                                while (*roomPosition == amphipod) {
                                    --roomPosition;
                                }

                                while (roomPosition >= destinationRoom->amphipods && *roomPosition != '.') {
                                    ++amphipodsToMove;
                                    --roomPosition;
                                }

                                int hallwaySpacesAvailable = 0;
                                int destinationRoomHallwayPosition = destinationRoom->hallwayPosition;

                                char hallway[HALLWAY_LENGTH];
                                strncpy(hallway, newBurrow.hallway, HALLWAY_LENGTH);

                                for (int k = destinationRoomHallwayPosition; k >= 0 && k < HALLWAY_LENGTH;) {
                                    if ((k == 0 || k == 10 || k % 2 == 1)) {
                                        if (hallway[k] == '.') {
                                            ++hallwaySpacesAvailable;
                                        } else {
                                            struct Room *hallwayAmphipodDestinationRoom = roomForAmphipod(&newBurrow, hallway[k]);

                                            if (roomAvailable(hallwayAmphipodDestinationRoom) && burrowHallwayPathClear(hallway, k, hallwayAmphipodDestinationRoom->hallwayPosition)) {
                                                hallway[k] = '.';
                                                ++hallwaySpacesAvailable;
                                            } else {
                                                break;
                                            }
                                        }
                                    } 

                                    if (destinationRoomHallwayPosition < j) {
                                        --k;
                                    } else {
                                        ++k;
                                    }
                                }

                                if (amphipodsToMove > hallwaySpacesAvailable) {
                                    hallwayPositionValid = false;
                                }
                            }

                            if (hallwayPositionValid) {
                                int energy = burrowState->energy + 
                                    (stepsOutOfRoom + abs(room->hallwayPosition - j)) * amphipodEnergy[amphipod - 'A'];

                                unsigned int hash = burrowHash(&newBurrow);

                                if (burrowStateCosts[hash] == 0 || energy < burrowStateCosts[hash]) {
                                    struct BurrowState *newBurrowState = (struct BurrowState *)calloc(1, sizeof(struct BurrowState));
                                    newBurrowState->burrow = newBurrow;
                                    newBurrowState->energy = energy;

                                    burrowStateCosts[hash] = energy;

                                    enqueueBurrowState(burrowStateQueue, newBurrowState);
                                }
                            }
                        }
                    }
                }
            }
        }

        free(burrowState);
    }

    while (burrowStateQueue->size) {
        free(dequeueBurrowState(burrowStateQueue));
    }

    free(burrowStateCosts);

    free(burrowStateQueue->data);
    free(burrowStateQueue);

    return energy;
}

bool readBurrow(char *filename, struct Burrow *burrow) {
    FILE *inputFile = fopen(filename, "r");

    if (inputFile) {
        char diagramLine[16];
        *burrow = (struct Burrow){
            { "..........." },
            { 2, 'A', "" },
            { 4, 'B', "" },
            { 6, 'C', "" },
            { 8, 'D', "" },
        };

        fgets(diagramLine, sizeof(diagramLine), inputFile);
        fgets(diagramLine, sizeof(diagramLine), inputFile);

        strncpy(burrow->hallway, diagramLine + 1, HALLWAY_LENGTH);

        for (int i = 0; i < ROOM_DEPTH; i++) {
            if (i == 0) {
                fscanf(inputFile, "###%c#%c#%c#%c###", 
                    &burrow->aRoom.amphipods[i], 
                    &burrow->bRoom.amphipods[i], 
                    &burrow->cRoom.amphipods[i], 
                    &burrow->dRoom.amphipods[i]);
            } else {
                fscanf(inputFile, "  #%c#%c#%c#%c#  ", 
                    &burrow->aRoom.amphipods[i], 
                    &burrow->bRoom.amphipods[i], 
                    &burrow->cRoom.amphipods[i], 
                    &burrow->dRoom.amphipods[i]);
            }
        }

        fclose(inputFile);

        return true;
    }

    return false;
}