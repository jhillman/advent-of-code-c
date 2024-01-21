#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

struct Location {
    int x;
    int y;
};

struct Keys {
    int bits;
    char characters[27];
    char *key;
};

void addKey(struct Keys *keys, char key) {
    int bit = 1 << (tolower(key) - 'a');

    if ((keys->bits & bit) == 0) {
        keys->bits |= bit;

        if (!keys->key) {
            keys->key = keys->characters;
        }

        *keys->key++ = tolower(key);
        *keys->key = '\0';
    }
}

struct Keys removeKey(struct Keys keys, char key) {
    int bit = 1 << (key - 'a');

    if (keys.bits & bit) {
        keys.bits &= ~bit;

        keys.key = strchr(keys.characters, key);

        while (keys.key && *keys.key) {
            *keys.key = *(keys.key + 1);

            ++keys.key;
        }
    }

    return keys;
}

struct StepsWithKeys {
    int keys;
    int steps;
};

struct StepsWithKeysList {
    int capacity;
    int count;
    struct StepsWithKeys *data;
};

int compare(const void *a, const void *b) {
    return ((struct StepsWithKeys *)a)->keys - ((struct StepsWithKeys *)b)->keys;
}

void addStepsWithKeys(struct StepsWithKeysList *list, struct StepsWithKeys stepsWithKeys) {
    if (list->count == list->capacity) {
        list->capacity += 100;
        list->data = (struct StepsWithKeys *)realloc(list->data, list->capacity * sizeof(struct StepsWithKeys));
    }

    list->data[list->count++] = stepsWithKeys;

    qsort(list->data, list->count, sizeof(struct StepsWithKeys), compare);
}

int findStepsWithKeys(struct StepsWithKeysList *list, int keys, int low, int high) {
    if (low > high) {
        return -1;
    }

    int middle = low + ((high - low) / 2);

    if (middle < list->count) {
        if (list->data[middle].keys == keys) {
            return list->data[middle].steps;
        }

        if (list->data[middle].keys > keys) {
            return findStepsWithKeys(list, keys, low, middle - 1);
        } else {
            return findStepsWithKeys(list, keys, middle + 1, high);
        }    
    }

    return -1;
}

enum MapType {
    COMPLETE,
    UPPER_LEFT,
    UPPER_RIGHT,
    LOWER_LEFT,
    LOWER_RIGHT
};

struct Map {
    int width;
    int height;
    
    struct Location entrance;
    struct Keys keys;

    char **grid;
    struct Location keyLocations[26];
    struct Keys keyDependencies[26];
    int keySteps[27][27];

    struct StepsWithKeysList ***stepsWithKeys;
};

struct LocationState {
    struct Location location;
    struct Keys keys;
    int steps;

    struct LocationState *next;
};

struct LocationStateQueue {
    struct LocationState *head;
    struct LocationState *tail;
    int size;
};

void enqueue(struct LocationStateQueue *queue, struct LocationState *state) {
    if (queue->tail) {
        queue->tail->next = state;
        queue->tail = state;
    } else {
        queue->head = queue->tail = state;
    }

    ++queue->size;
}

struct LocationState *dequeue(struct LocationStateQueue *queue) {
    struct LocationState *state = queue->head;

    if (queue->tail == state) {
        queue->head = queue->tail = NULL;
    } else if (state) {
        queue->head = state->next;
    }

    --queue->size;

    return state;
}

void freeLocationState(struct LocationState *state) {
    if (state->next) {
        freeLocationState(state->next);
    }

    free(state);
}

void freeLocationStateQueue(struct LocationStateQueue *queue) {
    if (queue->head) {
        freeLocationState(queue->head);
    }

    free(queue);
}

void findDependencies(struct Map *map) {
    int yDeltas[] = { -1, 1, 0, 0 };
    int xDeltas[] = { 0, 0, -1, 1 };
    struct LocationStateQueue *queue = (struct LocationStateQueue *)calloc(1, sizeof(struct LocationStateQueue));
    struct LocationState *state;
    struct LocationState *stepState;

    bool **visited = (bool **)calloc(map->height, sizeof(bool *));

    for (int y = 0; y < map->height; y++) {
        visited[y] = (bool *)calloc(map->width, sizeof(bool));
    }

    state = (struct LocationState *)calloc(1, sizeof(struct LocationState));
    state->location = map->entrance;

    enqueue(queue, state);
  
    while (queue->size) {
        state = dequeue(queue);

        visited[state->location.y][state->location.x] = true;

        for (int i = 0; i < 4; i++) {
            struct Location step = { state->location.x + xDeltas[i], state->location.y + yDeltas[i] };

            if (!(step.x >= 0 && step.x < map->width && step.y >= 0 && step.y < map->height) || visited[step.y][step.x]) {
                continue;
            }

            char character = map->grid[step.y][step.x];

            if (character == '#') {
                continue;
            }

            struct Keys keys = state->keys;

            if (islower(character)) {
                map->keyLocations[character - 'a'] = (struct Location){ step.x, step.y };
                map->keyDependencies[character - 'a'] = keys;
            }

            if (isalpha(character)) {
                addKey(&keys, character);
                addKey(&map->keys, character);
            }

            stepState = (struct LocationState *)calloc(1, sizeof(struct LocationState));
            stepState->location = step;
            stepState->keys = keys;

            enqueue(queue, stepState);
        }
    }

    freeLocationStateQueue(queue);

    for (int y = 0; y < map->height; y++) {
        free(visited[y]);
    }

    free(visited);
}

struct Map *getMap(char *input, enum MapType type) {
    FILE *inputFile = fopen(input, "r");
    struct Map *map = NULL;

    if (inputFile) {
        map = (struct Map *)calloc(1, sizeof(struct Map));
        char c;
        int key;
        int door;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++map->height;
            }

            if (!map->height) {
                ++map->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        map->grid = (char **)malloc(map->height * sizeof(char *));

        for (int y = 0; y < map->height; y++) {
            map->grid[y] = (char *)calloc(map->width + 1, sizeof(char));

            for (int x = 0; x < map->width; x++) {
                c = fgetc(inputFile);

                map->grid[y][x] = c;

                if (c == '@') {
                    map->entrance.x = x;
                    map->entrance.y = y;
                } else if (isalpha(c) && c < 'a' && type != COMPLETE) {
                    map->grid[y][x] = '.';
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        if (type != COMPLETE) {
            int xMiddle = map->width / 2;
            int yMiddle = map->height / 2;

            for (int y = yMiddle - 1; y <= yMiddle + 1; y++) {
                map->grid[y][xMiddle] = '#';
            }

            for (int x = xMiddle - 1; x <= xMiddle + 1; x++) {
                map->grid[yMiddle][x] = '#';
            }

            for (int y = 0; y < map->height; y++) {
                switch (type) {
                case UPPER_LEFT:
                    if (y > yMiddle + 1) {
                        free(map->grid[y]);
                    } else {
                        map->grid[y][xMiddle + 1] = '\0';
                    }
                    break;
                case UPPER_RIGHT:
                    if (y > yMiddle + 1) {
                        free(map->grid[y]);
                    } else {
                        memcpy(map->grid[y], map->grid[y] + xMiddle, xMiddle + 2);
                    }
                    break;
                case LOWER_LEFT:
                    if (y < yMiddle) {
                        free(map->grid[y]);
                    } else {
                        map->grid[y - yMiddle] = map->grid[y];
                        map->grid[y - yMiddle][xMiddle + 1] = '\0';
                    }
                    break;
                case LOWER_RIGHT:
                    if (y < yMiddle) {
                        free(map->grid[y]);
                    } else {
                        map->grid[y - yMiddle] = map->grid[y];
                        memcpy(map->grid[y - yMiddle], map->grid[y - yMiddle] + xMiddle, xMiddle + 2);
                    }
                    break;
                default:
                    break;
                }
            }

            map->width = xMiddle + 1;
            map->height = yMiddle + 1;

            switch (type) {
            case UPPER_LEFT:
                map->entrance = (struct Location){ xMiddle - 1, yMiddle - 1 };
                break;
            case UPPER_RIGHT:
                map->entrance = (struct Location){ 1, yMiddle - 1 };
                break;
            case LOWER_LEFT:
                map->entrance = (struct Location){ xMiddle - 1, 1 };
                break;
            case LOWER_RIGHT:
                map->entrance = (struct Location){ 1, 1 };
                break;
            default:
                break;
            }

            map->grid[map->entrance.y][map->entrance.x] = '@';
        }
    }

    findDependencies(map);

    return map;
}

int stepsWithKeys(struct Map *map, struct Location location, int keys) {
    struct StepsWithKeysList *stepsWithKeys = map->stepsWithKeys[location.y][location.x];

    return findStepsWithKeys(stepsWithKeys, keys, 0, stepsWithKeys->count);
}

void setStepsWithKeys(struct Map *map, struct Location location, int keys, int steps) {
    struct StepsWithKeysList *stepsWithKeys = map->stepsWithKeys[location.y][location.x];

    addStepsWithKeys(stepsWithKeys, (struct StepsWithKeys){ keys, steps });
}

int stepsToKey(struct Map *map, struct Location keyStart, struct Location keyEnd) {
    char key = map->grid[keyStart.y][keyStart.x];

    int keyStartIndex = (key == '@' ? 0 : (key - 'a' + 1));
    int keyEndIndex = map->grid[keyEnd.y][keyEnd.x] - 'a' + 1;
    int steps = map->keySteps[keyStartIndex][keyEndIndex];

    if (steps) {
        return steps;
    }

    int yDeltas[] = { -1, 1, 0, 0 };
    int xDeltas[] = { 0, 0, -1, 1 };
    struct LocationStateQueue *queue = (struct LocationStateQueue *)calloc(1, sizeof(struct LocationStateQueue));
    struct LocationState *state;
    struct LocationState *stepState;

    bool **visited = (bool **)calloc(map->height, sizeof(bool *));

    for (int y = 0; y < map->height; y++) {
        visited[y] = (bool *)calloc(map->width, sizeof(bool));
    }

    state = (struct LocationState *)calloc(1, sizeof(struct LocationState));
    state->location = keyStart;

    enqueue(queue, state);
  
    while (queue->size) {
        state = dequeue(queue);

        visited[state->location.y][state->location.x] = true;

        if (state->location.y == keyEnd.y && state->location.x == keyEnd.x) {
            steps = state->steps;

            break;
        }

        for (int i = 0; i < 4; i++) {
            struct Location step = { state->location.x + xDeltas[i], state->location.y + yDeltas[i] };

            if (!(step.x >= 0 && step.x < map->width && step.y >= 0 && step.y < map->height) || visited[step.y][step.x]) {
                continue;
            }

            if (map->grid[step.y][step.x] == '#') {
                continue;
            }

            stepState = (struct LocationState *)calloc(1, sizeof(struct LocationState));
            stepState->location = step;
            stepState->steps = state->steps + 1;

            enqueue(queue, stepState);
        }
    }

    map->keySteps[keyStartIndex][keyEndIndex] = steps;

    freeLocationStateQueue(queue);

    for (int y = 0; y < map->height; y++) {
        free(visited[y]);
    }

    free(visited);

    return steps;
}

int stepsToKeys(struct Map *map, struct Location location, struct Keys keys) {
    int steps = 0;

    if (!map->stepsWithKeys) {
        map->stepsWithKeys = (struct StepsWithKeysList ***)calloc(map->width + 1, sizeof(struct StepsWithKeysList **));
        
        for (int y = 0; y < map->height; y++) {
            map->stepsWithKeys[y] = (struct StepsWithKeysList **)calloc(map->width + 1, sizeof(struct StepsWithKeysList *));

            for (int x = 0; x < map->width; x++) {
                map->stepsWithKeys[y][x] = (struct StepsWithKeysList *)calloc(map->width + 1, sizeof(struct StepsWithKeysList));
            }
        }
    }

    if (keys.bits == 0) {
        return 0;
    }

    steps = stepsWithKeys(map, location, keys.bits);

    if (steps == -1) {
        steps = 10000;
        char *key = keys.characters;

        while (*key) {
            int bit = 1 << (*key - 'a');

            if ((map->keyDependencies[*key - 'a'].bits & keys.bits) == 0) {
                int keySteps = stepsToKey(map, location, map->keyLocations[*key - 'a']);

                keySteps += stepsToKeys(map, map->keyLocations[*key - 'a'], removeKey(keys, *key));

                if (keySteps < steps) {
                    steps = keySteps;
                }
            }

            ++key;
        }

        setStepsWithKeys(map, location, keys.bits, steps);
    }

    return steps;
}

void freeMap(struct Map *map) {
    for (int y = 0; y < map->height; y++) {
        free(map->grid[y]);

        if (map->stepsWithKeys) {
            for (int x = 0; x < map->width; x++) {
                if (map->stepsWithKeys[y][x]->data) {
                    free(map->stepsWithKeys[y][x]->data);
                }

                free(map->stepsWithKeys[y][x]);
            }

            free(map->stepsWithKeys[y]);
        }
    }

    free(map->grid);

    if (map->stepsWithKeys) {
        free(map->stepsWithKeys);
    }

    free(map);
}