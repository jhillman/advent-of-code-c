/* Day 25, part 1 = 562978 */

#include "../../aoc.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Component {
    int id;
    char name[4];
    char a;
    char b;
    char c;
};

struct Components {
    int capacity;
    int count;
    struct Component *list;
    bool ******connections;
};

struct Component initComponent(char *characters) {
    struct Component component;
    char *character = characters;

    strcpy(component.name, characters);
    component.a = *character++ - 'a';
    component.b = *character++ - 'a';
    component.c = *character++ - 'a';

    return component;
}

void connectComponents(struct Components *components, struct Component one, struct Component two) {
    int range = ('z' - 'a') + 1;

    if (components->count == components->capacity || components->count + 1 == components->capacity) {
        components->capacity += 10;
        components->list = (struct Component *)realloc(components->list, components->capacity * sizeof(struct Component));
    }

    if (!components->connections) {
        components->connections = (bool ******)calloc(range, sizeof(bool *****));
    }

    for (int i = 0; i < 2; i++) {
        struct Component lhs = i == 0 ? one : two;
        struct Component rhs = i == 0 ? two : one;
        bool addToList = false;

        if (!components->connections[lhs.a]) {
            components->connections[lhs.a] = (bool *****)calloc(range, sizeof(bool ****));
        }

        if (!components->connections[lhs.a][lhs.b]) {
            components->connections[lhs.a][lhs.b] = (bool ****)calloc(range, sizeof(bool ***));
        }

        if (!components->connections[lhs.a][lhs.b][lhs.c]) {
            components->connections[lhs.a][lhs.b][lhs.c] = (bool ***)calloc(range, sizeof(bool **));
            addToList = true;
        }

        if (!components->connections[lhs.a][lhs.b][lhs.c][rhs.a]) {
            components->connections[lhs.a][lhs.b][lhs.c][rhs.a] = (bool **)calloc(range, sizeof(bool *));
        }

        if (!components->connections[lhs.a][lhs.b][lhs.c][rhs.a][rhs.b]) {
            components->connections[lhs.a][lhs.b][lhs.c][rhs.a][rhs.b] = (bool *)calloc(range, sizeof(bool));
        }

        if (addToList) {
            lhs.id = components->count;
            components->list[components->count++] = lhs;
        }

        components->connections[lhs.a][lhs.b][lhs.c][rhs.a][rhs.b][rhs.c] = true;
    }
}

bool componentsConnected(struct Components *components, struct Component one, struct Component two) {
    if (!components->connections[one.a][one.b][one.c][two.a]) {
        return false;
    }

    if (!components->connections[one.a][one.b][one.c][two.a][two.b]) {
        return false;
    }

    return components->connections[one.a][one.b][one.c][two.a][two.b][two.c];
}

void freeComponments(struct Components *components) {
    int range = ('z' - 'a') + 1;

    if (components->connections) {
        for (int a1 = 0; a1 < range; a1++) {
            if (!components->connections[a1]) {
                continue;
            }            

            for (int b1 = 0; b1 < range; b1++) {
                if (!components->connections[a1][b1]) {
                    continue;
                }

                for (int c1 = 0; c1 < range; c1++) {
                    if (!components->connections[a1][b1][c1]) {
                        continue;
                    }

                    for (int a2 = 0; a2 < range; a2++) {
                        if (!components->connections[a1][b1][c1][a2]) {
                            continue;
                        }            

                        for (int b2 = 0; b2 < range; b2++) {
                            if (!components->connections[a1][b1][c1][a2][b2]) {
                                continue;
                            }

                            free(components->connections[a1][b1][c1][a2][b2]);
                        }

                        free(components->connections[a1][b1][c1][a2]);
                    }

                    free(components->connections[a1][b1][c1]);
                }

                free(components->connections[a1][b1]);
            }

            free(components->connections[a1]);
        }

        free(components->connections);
    }

    if (components->list) {
        free(components->list);
    }

    free(components);
}

struct Todo {
    int id;
    struct Todo *next;
};

struct TodoList {
    int count;
    struct Todo *first;
    struct Todo *last;
};

void addTodo(struct TodoList *list, int id) {
    struct Todo *todo = (struct Todo *)calloc(1, sizeof(struct Todo));
    todo->id = id;

    if (list->last) {
        list->last->next = todo;
        list->last = todo;
    } else {
        list->first = list->last = todo;
    }

    ++list->count;
}

struct Todo *nextTodo(struct TodoList *list) {
    struct Todo *todo = list->first;

    --list->count;

    if (list->last == todo) {
        list->first = list->last = NULL;
    } else if (todo) {
        list->first = todo->next;
    }

    return todo;
}

void freeTodoList(struct TodoList *list) {
    struct Todo *todo = list->first;
    struct Todo *next;

    while (todo) {
        next = todo->next;

        free(todo);
        todo = next;
    }

    free(list);
}

int findPath(struct Components *components, int **flow, struct Component start, struct Component end) {
    int steps = 0;
    struct TodoList *todo = (struct TodoList *)calloc(1, sizeof(struct TodoList));
    int *from = (int *)malloc(components->count * sizeof(int));

    for (int i = 0; i < components->count; i++) {
        from[i] = -1;
    }

    from[start.id] = 0;
    addTodo(todo, start.id);

    while (todo->count > 0 && from[end.id] == -1) {
        struct Component current = components->list[nextTodo(todo)->id];
        ++steps;

        for (int i = 0; i < components->count; i++) {
            struct Component other = components->list[i];
            
            if (componentsConnected(components, current, other) && !flow[current.id][other.id] && from[other.id] == -1) {
                from[other.id] = current.id;
                addTodo(todo, other.id);
            }
        }
    }

    if (from[end.id] >= 0) {
        int id = end.id;

        while (id != start.id) {
            ++flow[from[id]][id];
            --flow[id][from[id]];

            id = from[id];
        }

        steps = 0;
    }

    freeTodoList(todo);
    free(from);

    return steps;
}

int componentGroupSize(struct Components *components, struct Component start, struct Component end) {
    int **flow = (int **)malloc(components->count * sizeof(int *));
    int max = 0;
    int groupSize = 0;

    for (int i = 0; i < components->count; i++) {
        flow[i] = (int *)calloc(components->count, sizeof(int));
    }

    while (true) {
        int size = findPath(components, flow, start, end);

        if (size == 0) {
            ++max;
        } else {
            if (max == 3) {
                groupSize = size;
            }

            break;
        }
    }

    for (int i = 0; i < components->count; i++) {
        free(flow[i]);
    }

    free(flow);

    return groupSize;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    if (inputFile) {
        char line[64];
        char *characters;
        struct Components *components = (struct Components *)calloc(1, sizeof(struct Components));
        struct Component lhs;
        struct Component rhs;
        int answer = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            characters = strtok(line, ": ");

            lhs = initComponent(characters);

            characters = strtok(NULL, " ");

            while (characters) {
                rhs = initComponent(characters);

                connectComponents(components, lhs, rhs);

                characters = strtok(NULL, " ");
            }
        }

        fclose(inputFile);

        struct Component start = *components->list;

        for (int i = 1; !answer && i < components->count; i++) {
            int size = componentGroupSize(components, start, components->list[i]);

            if (size) {
                answer = size * (components->count - size);
            }
        }

        freeComponments(components);

        print(aoc, INT, &answer);
    }

    return 0;
}
