/* Day 5, part 2 = 5448 */

#include "../../aoc.h"
#include "updates.h"

struct Pages *globalPages = NULL;

int comparePages(const void *a, const void *b) {
    int page1 = *(int *)a;
    int page2 = *(int *)b;

    if (findPage(&globalPages[page1], page2, 0, globalPages[page1].count)) {
        return 1;
    } else {
        return -1;
    }
}

void processUpdate(struct Pages *pages, int *update, int length, bool valid, int *sum) {
    if (globalPages == NULL) {
        globalPages = pages;
    }

    if (!valid) {
        qsort(update, length, sizeof(int), comparePages);

        *sum += update[length / 2];
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = 0;

    processUpdates(aoc.input, &answer);

    print(aoc, INT, &answer);

    return 0;
}
