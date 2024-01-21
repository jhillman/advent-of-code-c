/* Day 7, part 1 = 1517599 */

#include "../../aoc.h"
#include "filesystem.h"

void sumDirectoriesUnderLimit(struct Directory *directory, int limit, int *sum) {
    if (directory->totalSize <= limit) {
        *sum += directory->totalSize;
    }

    for (int i = 0; i < directory->directoryCount; i++) {
        sumDirectoriesUnderLimit(directory->directories[i], limit, sum);
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Directory *root = getRoot(aoc.input);
    int answer = 0;

    if (root) {
        int sum = 0;
        sumDirectoriesUnderLimit(root, 100000, &sum);
        freeDirectory(root);

        answer = sum;
    }

    print(aoc, INT, &answer);

    return 0;
}
