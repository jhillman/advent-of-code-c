#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

struct Step {
    char id;
    int prerequisiteCount;
    struct Step *prerequisites[26];
};

struct StepData {
    int stepCount;
    struct Step steps[26];
    
    char completed[27];
    char *step;
};

bool stepsRemaining(struct StepData *data) {
    return strlen(data->completed) < data->stepCount;
}

struct Step *getPrerequisite(struct Step *step, char prerequisiteId) {
    return step->prerequisites[prerequisiteId - 'A'];
}

struct Step *getStep(struct StepData *data, char id) {
    return &data->steps[id - 'A'];
}

void addPrerequisite(struct StepData *data, char stepId, char prerequisiteId) {
    struct Step *step = getStep(data, stepId);

    step->prerequisites[prerequisiteId - 'A'] = getStep(data, prerequisiteId);
    ++step->prerequisiteCount;
}

void removePrerequisite(struct StepData *data, char stepId, char prerequisiteId) {
    struct Step *step = getStep(data, stepId);

    if (step->prerequisites[prerequisiteId - 'A']) {
        step->prerequisites[prerequisiteId - 'A'] = NULL;
        --step->prerequisiteCount;
    }
}

char lowestId(struct StepData *data) {
    struct Step *step;
    char lowest = '\0';

    for (int id = 'A'; id <= 'Z'; id++) {
        step = getStep(data, id);

        if (step->id && !strchr(data->completed, step->id) && 
            step->prerequisiteCount == 0 && (!lowest || step->id < lowest)) {
            lowest = step->id;
        }
    }    
    
    return lowest;
}

void completeStep(struct StepData *data, char stepId) {
    *data->step++ = stepId;
    *data->step = '\0';
}

struct Worker {
    char stepId;
    int timeRemaining;
};

bool stillWorking(struct Worker *workers) {
    bool working = false;

    for (int i = 0; !working && i < 5; i++) {
        working = workers[i].timeRemaining > 0;
    }

    return working;
}

struct StepData *getStepData(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct StepData *data = NULL;

    if (inputFile) {
        char p;
        char s;
        struct Step *prerequisite;
        struct Step *step;

        data = (struct StepData *)calloc(1, sizeof(struct StepData));

        strcpy(data->completed, "");

        data->step = data->completed;

        while (fscanf(inputFile, "Step %c must be finished before step %c can begin.\n", &p, &s) == 2) {
            prerequisite = getStep(data, p);
            step = getStep(data, s);

            if (!prerequisite->id) {
                ++data->stepCount;
            }

            if (!step->id) {
                ++data->stepCount;
            }

            prerequisite->id = p;
            step->id = s;

            addPrerequisite(data, step->id, prerequisite->id);
        }

        fclose(inputFile);
    }

    return data;
}