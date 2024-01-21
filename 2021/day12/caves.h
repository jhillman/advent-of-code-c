#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum CaveType {
    START,
    END,
    BIG,
    SMALL
};

struct Cave {
    char name[8];
    enum CaveType type;
    int flag;
    
    struct Cave **connections;
    int connectionCount;
};

struct CaveData {
    struct Cave **caves;
    int caveCount;

    struct Cave *start;
};

struct Cave *getCave(struct CaveData *data, char *name) {
    struct Cave *cave = NULL;

    for (int i = 0; i < data->caveCount; i++) {
        if (strcmp(name, data->caves[i]->name) == 0) {
            cave = data->caves[i];
            break;
        }
    }

    if (!cave) {
        ++data->caveCount;
        data->caves = (struct Cave **)realloc(data->caves, data->caveCount * sizeof(struct Cave *));

        cave = (struct Cave *)calloc(1, sizeof(struct Cave));

        strcpy(cave->name, name);

        if (strcmp(name, "start") == 0) {
            cave->type = START;
            data->start = cave;
        } else if (strcmp(name, "end") == 0) {
            cave->type = END;
        } else {
            cave->type = isupper(*name) ? BIG : SMALL;
        }

        cave->flag = 1 << (data->caveCount - 1);

        data->caves[data->caveCount - 1] = cave;
    }

    return cave;
}

void addConnection(struct Cave *firstCave, struct Cave *secondCave) {
    if (secondCave->type != START && firstCave->type != END) {
        ++firstCave->connectionCount;
        firstCave->connections = (struct Cave **)realloc(firstCave->connections, firstCave->connectionCount * sizeof(struct Cave *));

        firstCave->connections[firstCave->connectionCount - 1] = secondCave;
    }
}

void addConnections(struct CaveData * data, char *firstName, char *secondName) {
    struct Cave *firstCave = getCave(data, firstName);
    struct Cave *secondCave = getCave(data, secondName);

    addConnection(firstCave, secondCave);
    addConnection(secondCave, firstCave);
}

int pathCount(struct CaveData *data, struct Cave *cave, int visitedSmallCaves, bool allowSecondVisit) {
    int paths = 0;

    if (cave->type == END) {
        return 1;
    }

    int currentPathSmallCaves = visitedSmallCaves;

    if (cave->type == SMALL) {
        currentPathSmallCaves |= cave->flag;
    }

    bool secondVisit = cave->type == SMALL && visitedSmallCaves & cave->flag;

    for (int i = 0; i < cave->connectionCount; i++) {
        if (!(visitedSmallCaves & cave->connections[i]->flag) || (allowSecondVisit && !secondVisit)) {
            paths += pathCount(data, cave->connections[i], currentPathSmallCaves, allowSecondVisit && !secondVisit);
        }
    }

    return paths;
}

struct CaveData *getCaveData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct CaveData *data = (struct CaveData *)calloc(1, sizeof(struct CaveData));

        char caveConnection[16];

        while (fscanf(inputFile, "%s", caveConnection) > 0) {
            addConnections(data, strtok(caveConnection, "-"), strtok(NULL, "-"));
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeCaveData(struct CaveData *data) {
    for (int i = 0; i < data->caveCount; i++) {
        if (data->caves[i]->connections) {
            free(data->caves[i]->connections);
        }

        free(data->caves[i]);
    }

    free(data->caves);
    free(data);
}