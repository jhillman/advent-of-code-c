#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum Condition {
    NONE,
    LESS_THAN,
    GREATER_THAN
};

struct Rule {
    char category;
    enum Condition condition;
    long value;
    char result[8];
};

struct Rules {
    int capacity;
    int count;
    struct Rule * data;
};

void addRule(struct Rules *rules, struct Rule rule) {
    if (rules->count == rules->capacity) {
        rules->capacity += 5;
        rules->data = (struct Rule *)realloc(rules->data, rules->capacity * sizeof(struct Rule));
    }

    rules->data[rules->count++] = rule;
}

struct Workflow {
    char id[8];
    struct Rules *rules;
};

struct Workflows {
    int capacity;
    int count;
    struct Workflow **data;
};

void addWorkflow(struct Workflows *workflows, struct Workflow *workflow) {
    if (workflows->count == workflows->capacity) {
        workflows->capacity += 5;
        workflows->data = (struct Workflow **)realloc(workflows->data, workflows->capacity * sizeof(struct Workflow *));
    }

    workflows->data[workflows->count++] = workflow;
}

struct Workflow *findWorkflow(struct Workflows *workflows, char id[8]) {
    struct Workflow *workflow = NULL;

    for (int i = 0; !workflow && i < workflows->count; i++) {
        if (strcmp(workflows->data[i]->id, id) == 0) {
            workflow = workflows->data[i];
        }
    }

    return workflow;
}

struct Part {
    long x;
    long m;
    long a;
    long s;
};

struct Parts {
    int capacity;
    int count;
    struct Part *data;
};

void addPart(struct Parts *parts, struct Part part) {
    if (parts->count == parts->capacity) {
        parts->capacity += 5;
        parts->data = (struct Part *)realloc(parts->data, parts->capacity * sizeof(struct Part ));
    }

    parts->data[parts->count++] = part;
}

long evaluatePart(struct Workflows *workflows, char id[8], struct Part part) {
    char partCategories[] = "xmas";
    long result = -1;

    if (*id == 'A') {
        result = part.x + part.m + part.a + part.s;
    } else if (*id == 'R') {
        result = 0;
    }

    struct Workflow *workflow = findWorkflow(workflows, id);

    if (workflow) {
        for (int i = 0; result < 0 && i < workflow->rules->count; i++) {
            struct Rule rule = workflow->rules->data[i];
            long partCategory;
            char *category = strchr(partCategories, rule.category);

            if (category) {
                partCategory = *((long *)&part + (category - partCategories));
            }

            switch (rule.condition) {
            case NONE:
                if (*rule.result == 'A') {
                    result = part.x + part.m + part.a + part.s;
                } else if (*rule.result == 'R') {
                    result = 0;
                } else {
                    result = evaluatePart(workflows, rule.result, part);
                }
                break;
            case LESS_THAN:
                if (partCategory < rule.value) {
                    result = evaluatePart(workflows, rule.result, part);
                }
                break;
            case GREATER_THAN:
                if (partCategory > rule.value) {
                    result = evaluatePart(workflows, rule.result, part);
                }
                break;
            }
        }
    }

    return result;
}

struct SortingData {
    struct Workflows *workflows;
    struct Parts *parts;
};

struct SortingData *getSortingData(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct SortingData *data = NULL;

    if (inputFile) {
        char line[64];
        char *c;
        struct Rule rule;
        struct Workflow *workflow;
        bool readingParts = false;
        struct Part part;

        data = (struct SortingData *)calloc(1, sizeof(struct SortingData));
        data->workflows = (struct Workflows *)calloc(1, sizeof(struct Workflows));
        data->parts = (struct Parts *)calloc(1, sizeof(struct Parts));

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            if (readingParts) {
                sscanf(line, "{x=%ld,m=%ld,a=%ld,s=%ld}", &part.x, &part.m, &part.a, &part.s);

                addPart(data->parts, part);
            } else if (strlen(line) == 0) {
                readingParts = true;
            } else {
                c = strchr(line, '{');

                if (c) {
                    workflow = (struct Workflow *)calloc(1, sizeof(struct Workflow));
                    workflow->rules = (struct Rules *)calloc(1, sizeof(struct Rules));

                    *c++ = '\0';

                    strcpy(workflow->id, line);

                    c = strtok(c, ",");

                    while (c) {
                        c[strcspn(c, "}")] = 0;

                        rule.category = '-';
                        rule.condition = NONE;
                        rule.value = 0;
                        strcpy(rule.result, "");

                        if (strchr(c, ':')) {
                            rule.category = *c;
                            rule.condition = *(c + 1) == '<' ? LESS_THAN : GREATER_THAN;
                            sscanf(c + 2, "%ld:%s", &rule.value, rule.result);
                        } else {
                            strcpy(rule.result, c);
                        }

                        addRule(workflow->rules, rule);

                        c = strtok(NULL, ",");
                    }

                    addWorkflow(data->workflows, workflow);
                }
            }
        }

        fclose(inputFile);
    }

    return data;
}

void freeSortingData(struct SortingData *data) {
    for (int i = 0; i < data->workflows->count; i++) {
        free(data->workflows->data[i]->rules->data);
        free(data->workflows->data[i]->rules);
    }

    free(data->workflows->data);
    free(data->workflows);

    free(data->parts->data);
    free(data->parts);
}

