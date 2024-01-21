#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Blueprint {
    int id;
    int oreRobotOreCost;
    int clayRobotOreCost;
    int obsidianRobotOreCost;
    int obsidianRobotClayCost;
    int geodeRobotOreCost;
    int geodeRobotObsidianCost;
};

struct ProductionState {
    int minutes;
    int oreRobotCount;
    int clayRobotCount;
    int obsidianRobotCount;
    int geodeRobotCount;
    int ore;
    int clay;
    int obsidian;
    int geodes;
};

struct ProductionStateQueue {
    struct ProductionState *data;
    int capacity;
    int size;
    int index;
};

void addProductionState(struct ProductionStateQueue *queue, struct ProductionState state) {
    if (queue->capacity == 0) {
        queue->capacity = 1000000;
        queue->data = (struct ProductionState *)malloc(queue->capacity * sizeof(struct ProductionState));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 1000000;
        queue->data = (struct ProductionState *)realloc(queue->data, queue->capacity * sizeof(struct ProductionState));
    }

    queue->data[queue->size++] = state;
}

struct ProductionState removeProductionState(struct ProductionStateQueue *queue) {
    return queue->data[queue->index++];
}

long hashProductionState(struct ProductionState state) {
    long hash = state.minutes;

    hash *= 100;
    hash += state.oreRobotCount;
    hash *= 100;
    hash += state.clayRobotCount;
    hash *= 100;
    hash += state.obsidianRobotCount;
    hash *= 100;
    hash += state.geodeRobotCount;
    hash *= 100;
    hash += state.ore;
    hash *= 100;
    hash += state.clay;
    hash *= 100;
    hash += state.obsidian;
    hash *= 100;
    hash += state.geodes;

    return hash % 10000000019;
}

int updateMaterial(int current, int minutes, int robotCount, int materialCost) {
    int productionRequired = minutes * materialCost;
    int projectedProduction = robotCount * (minutes - 1);

    return min(current, productionRequired - projectedProduction);
}

int maxGeodes(struct Blueprint blueprint, int minutes) {
    struct ProductionStateQueue *queue = (struct ProductionStateQueue *)calloc(1, sizeof(struct ProductionStateQueue));
    bool *visited = (bool *)calloc(10000000000, sizeof(bool));
    int geodeCount = 0;

    addProductionState(queue, (struct ProductionState){ minutes, 1, 0, 0, 0, 0, 0, 0, 0 });

    int maxOreNeeded = max(
        max(blueprint.oreRobotOreCost, blueprint.clayRobotOreCost),
        max(blueprint.obsidianRobotOreCost, blueprint.geodeRobotOreCost)
    );

    while (queue->size - queue->index > 0) {
        struct ProductionState state = removeProductionState(queue);

        long stateHash = hashProductionState(state);

        if (visited[stateHash]) {
            continue;
        }

        visited[stateHash] = true;

        if (state.minutes <= 0) {
            geodeCount = max(geodeCount, state.geodes);
            continue;
        }

        state.ore = updateMaterial(state.ore, state.minutes, state.oreRobotCount, maxOreNeeded);
        state.clay = updateMaterial(state.clay, state.minutes, state.clayRobotCount, blueprint.obsidianRobotClayCost);
        state.obsidian = updateMaterial(state.obsidian, state.minutes, state.obsidianRobotCount, blueprint.geodeRobotObsidianCost);

        if (state.ore >= blueprint.oreRobotOreCost && state.oreRobotCount < maxOreNeeded) {
            addProductionState(queue, (struct ProductionState) {
                state.minutes - 1,
                state.oreRobotCount + 1,
                state.clayRobotCount,
                state.obsidianRobotCount,
                state.geodeRobotCount,
                state.ore - blueprint.oreRobotOreCost + state.oreRobotCount,
                state.clay + state.clayRobotCount,
                state.obsidian + state.obsidianRobotCount,
                state.geodes + state.geodeRobotCount
            });
        }

        if (state.ore >= blueprint.clayRobotOreCost && state.clayRobotCount < blueprint.obsidianRobotClayCost) {
            addProductionState(queue, (struct ProductionState) {
                state.minutes - 1,
                state.oreRobotCount,
                state.clayRobotCount + 1,
                state.obsidianRobotCount,
                state.geodeRobotCount,
                state.ore - blueprint.clayRobotOreCost + state.oreRobotCount,
                state.clay + state.clayRobotCount,
                state.obsidian + state.obsidianRobotCount,
                state.geodes + state.geodeRobotCount
            });
        }

        if (state.ore >= blueprint.obsidianRobotOreCost && state.clay >= blueprint.obsidianRobotClayCost && state.obsidianRobotCount < blueprint.geodeRobotObsidianCost) {
            addProductionState(queue, (struct ProductionState) {
                state.minutes - 1,
                state.oreRobotCount,
                state.clayRobotCount,
                state.obsidianRobotCount + 1,
                state.geodeRobotCount,
                state.ore - blueprint.obsidianRobotOreCost + state.oreRobotCount,
                state.clay - blueprint.obsidianRobotClayCost + state.clayRobotCount,
                state.obsidian + state.obsidianRobotCount,
                state.geodes + state.geodeRobotCount
            });
        }

        if (state.ore >= blueprint.geodeRobotOreCost && state.obsidian >= blueprint.geodeRobotObsidianCost) {
            addProductionState(queue, (struct ProductionState) {
                state.minutes - 1,
                state.oreRobotCount,
                state.clayRobotCount,
                state.obsidianRobotCount,
                state.geodeRobotCount + 1,
                state.ore - blueprint.geodeRobotOreCost + state.oreRobotCount,
                state.clay + state.clayRobotCount,
                state.obsidian - blueprint.geodeRobotObsidianCost + state.obsidianRobotCount,
                state.geodes + state.geodeRobotCount
            });
        }

        addProductionState(queue, (struct ProductionState) {
            state.minutes - 1,
            state.oreRobotCount,
            state.clayRobotCount,
            state.obsidianRobotCount,
            state.geodeRobotCount,
            state.ore + state.oreRobotCount,
            state.clay + state.clayRobotCount,
            state.obsidian + state.obsidianRobotCount,
            state.geodes + state.geodeRobotCount
        });
    }

    free(visited);

    free(queue->data);
    free(queue);

    return geodeCount;
}

struct Blueprint *getBlueprints(char *input, int *blueprintCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        int count = 0;
        struct Blueprint *blueprints;
        struct Blueprint blueprint;
        char c;

        while (fscanf(inputFile,
            "Blueprint %d: Each ore robot costs %d ore.\
             Each clay robot costs %d ore.\
             Each obsidian robot costs %d ore and %d clay.\
             Each geode robot costs %d ore and %d obsidian.%c",
            &blueprint.id,
            &blueprint.oreRobotOreCost,
            &blueprint.clayRobotOreCost,
            &blueprint.obsidianRobotOreCost,
            &blueprint.obsidianRobotClayCost,
            &blueprint.geodeRobotOreCost,
            &blueprint.geodeRobotObsidianCost,
            &c) >= 7) {
            if (count == 0) {
                blueprints = (struct Blueprint *)malloc(sizeof(struct Blueprint));
            } else {
                blueprints = (struct Blueprint *)realloc(blueprints, (count + 1) * sizeof(struct Blueprint));
            }

            blueprints[count++] = blueprint;
        }

        fclose(inputFile);

        *blueprintCount = count;

        return blueprints;
    }

    return NULL;
}