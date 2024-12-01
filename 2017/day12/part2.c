/* Day 12, part 2 = 221 */

#include "../../aoc.h"
#include "nodes.h"

struct Groups {
    int capacity;
    int count;
    char **data;
};

int compare(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

char *findGroup(struct Groups *groups, char *group, int low, int high) {
    if (groups->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < groups->count) {
        int result = strcmp(group, groups->data[middle]);

        if (result == 0) {
            return groups->data[middle];
        } else if (result < 0) {
            return findGroup(groups, group, low, middle - 1);
        } else {
            return findGroup(groups, group, middle + 1, high);
        }
    }

    return NULL;
}

void addGroup(struct Groups *groups, char *group) {
    if (findGroup(groups, group, 0, groups->count) == NULL) {
        if (groups->count == groups->capacity) {
            groups->capacity += 5;
            groups->data = (char **)realloc(groups->data, groups->capacity * sizeof(char **));
        }

        groups->data[groups->count] = malloc((strlen(group) + 1) * sizeof(char));
        strcpy(groups->data[groups->count++], group);

        qsort(groups->data, groups->count, sizeof(char *), compare);
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Nodes *nodes = getNodes(aoc.input);
    int answer = 0;

    if (nodes) {
        bool *visited = (bool *)malloc(nodes->count * sizeof(bool));
        struct Groups *groups = (struct Groups *)calloc(1, sizeof(struct Groups));
        char group[1024];

        for (int i = 0; i < nodes->count; i++) {
            memset(visited, false, nodes->count * sizeof(bool));

            connectedCount(nodes, i, visited);
            strcpy(group, "");

            for (int j = 0; j < nodes->count; j++) {
                if (visited[j]) {
                    sprintf(group + strlen(group), "%d", j);
                }
            }

            addGroup(groups, group);
        }

        answer = groups->count;

        for (int i = 0; i < groups->count; i++) {
            free(groups->data[i]);
        }

        free(groups->data);
        free(groups);

        free(visited);
        freeNodes(nodes);
    }

    print(aoc, INT, &answer);

    return 0;
}
