#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct ValveName {
    char name[3];
    char one;
    char two;
};

struct Valve {
    struct ValveName name;
    int flowRate;
    struct ValveName *connections;
    int connectionCount;
};

struct Valve *valveWithName(struct Valve **valves, struct ValveName name) {
    return &valves[name.one - 'A'][name.two - 'A'];
}

void setDistance(int ****distances, struct ValveName first, struct ValveName second, int distance) {
    distances[first.one - 'A'][first.two - 'A'][second.one - 'A'][second.two - 'A'] = distance;
}

int distance(int ****distances, struct ValveName first, struct ValveName second) {
    return distances[first.one - 'A'][first.two - 'A'][second.one - 'A'][second.two - 'A'];
}

int findMaxPressure(struct ValveName valve, int totalTime, bool flowingValves[26][26], struct Valve **valves, 
                    struct ValveName *valveNames, int valveCount, int ****distances, int timePassed, int totalPressure) {
    int maxPressure = -1;

    for (int i = 0; i < valveCount; i++) {
        int distanceToValve = distance(distances, valve, valveNames[i]);

        if (!flowingValves[valveNames[i].one - 'A'][valveNames[i].two - 'A'] && timePassed + distanceToValve + 1 < totalTime) {
            bool newPressureingValves[26][26];
            memcpy(newPressureingValves, flowingValves, sizeof(newPressureingValves));

            newPressureingValves[valveNames[i].one - 'A'][valveNames[i].two - 'A'] = true;

            int pressure = findMaxPressure(valveNames[i], totalTime, newPressureingValves, valves, valveNames, valveCount, distances, 
                timePassed + distanceToValve + 1, 
                totalPressure + ((totalTime - timePassed - distanceToValve - 1) * valveWithName(valves, valveNames[i])->flowRate));

            maxPressure = max(maxPressure, pressure);
        }
    }

    return maxPressure != -1 ? maxPressure : totalPressure;
}

struct ValveData {
    struct Valve **valves;
    struct ValveName *valveNames;
    int valveCount;
    struct ValveName *flowingValveNames;
    int flowingValveCount;
    int ****distances;
};

struct ValveData *getValveData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct ValveData *data = (struct ValveData *)calloc(1, sizeof(struct ValveData));

        char line[128];
        struct ValveName name;
        int flowRate;
        char *connections;
        char *connection;

        data->valves = (struct Valve **)malloc(26 * sizeof(struct Valve *));

        for (int i = 0; i < 26; i++) {
            data->valves[i] = (struct Valve *)malloc(26 * sizeof(struct Valve));
        }

        data->distances = (int ****)malloc(26 * sizeof(int ***));

        for (int i = 0; i < 26; i++) {
            data->distances[i] = (int ***)malloc(26 * sizeof(int **));
    
            for (int j = 0; j < 26; j++) {
                data->distances[i][j] = (int **)malloc(26 * sizeof(int *));

                for (int k = 0; k < 26; k++) {
                    data->distances[i][j][k] = (int *)calloc(26, sizeof(int));
                }
            }
        }

        while (!feof(inputFile)) {
            fgets(line, sizeof(line), inputFile);
            line[strcspn(line, "\n")] = 0;

            sscanf(line, "Valve %c%c has flow rate=%d;", &name.one, &name.two, &flowRate);

            sprintf(name.name, "%c%c", name.one, name.two);
            name.name[2] = 0;

            if (data->valveNames) {
                data->valveNames = (struct ValveName *)realloc(data->valveNames, (data->valveCount + 1) * sizeof(struct ValveName));
            } else {
                data->valveNames = (struct ValveName *)malloc(sizeof(struct ValveName));
            }

            data->valveNames[data->valveCount++] = name;

            if (flowRate > 0) {
                if (data->flowingValveNames) {
                    data->flowingValveNames = (struct ValveName *)realloc(data->flowingValveNames, (data->flowingValveCount + 1) * sizeof(struct ValveName));
                } else {
                    data->flowingValveNames = (struct ValveName *)malloc(sizeof(struct ValveName));
                }

                data->flowingValveNames[data->flowingValveCount++] = name;
            }

            struct Valve *valve = valveWithName(data->valves, name);

            valve->flowRate = flowRate;

            connections = strstr(line, "valve") + 6;

            if (*connections == ' ') {
                ++connections;
            }

            connection = strtok(connections, ", ");

            while (connection) {
                if (valve->connections) {
                    valve->connections = (struct ValveName *)realloc(valve->connections, (valve->connectionCount + 1) * sizeof(struct ValveName));
                } else {
                    valve->connections = (struct ValveName *)malloc(sizeof(struct ValveName));
                }

                sscanf(connection, "%c%c", &name.one, &name.two);

                sprintf(name.name, "%c%c", name.one, name.two);
                name.name[2] = 0;

                valve->connections[valve->connectionCount++] = name;

                connection = strtok(NULL, ", ");
            }
        }

        fclose(inputFile);

        for (int i = 0; i < data->valveCount; i++) {
            for (int j = 0; j < data->valveCount; j++) {
                setDistance(data->distances, data->valveNames[i], data->valveNames[j], 1000);
            }

            struct Valve *valve = valveWithName(data->valves, data->valveNames[i]);

            for (int j = 0; j < valve->connectionCount; j++) {
                setDistance(data->distances, data->valveNames[i], valve->connections[j], 1);
                setDistance(data->distances, valve->connections[j], data->valveNames[i], 1);
            }
        }

        for (int k = 0; k < data->valveCount; k++) {
            for (int i = 0; i < data->valveCount; i++) {
                for (int j = 0; j < data->valveCount; j++) {
                    setDistance(data->distances, data->valveNames[i], data->valveNames[j], 
                        min(distance(data->distances, data->valveNames[i], data->valveNames[j]), 
                            distance(data->distances, data->valveNames[i], data->valveNames[k]) + distance(data->distances, data->valveNames[k], data->valveNames[j])));
                }
            }
        }

        return data;
    }

    return NULL;
}

void freeValveData(struct ValveData *data) {
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            if (data->valves[i][j].connections) {
                free(data->valves[i][j].connections);
            }
        }

        free(data->valves[i]);
    }

    free(data->valves);
    free(data->valveNames);
    free(data->flowingValveNames);

    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            for (int k = 0; k < 26; k++) {
                free(data->distances[i][j][k]);
            }

            free(data->distances[i][j]);
        }

        free(data->distances[i]);
    }

    free(data);
}
