/* Day 1, part 1 = 2344935 */

#include "../../aoc.h"
#include "lists.h"

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int *listOne;
    int *listTwo;
    int numberCount;
    int answer = 0;

    getLists(aoc.input, &listOne, &listTwo, &numberCount);

    if (listOne && listTwo) {
        int differences = 0;

        qsort(listOne, numberCount, sizeof(int), compare);
        qsort(listTwo, numberCount, sizeof(int), compare);

        for (int i = 0; i < numberCount; i++) {
            differences += abs(listOne[i] - listTwo[i]);
        }

        answer = differences;

        free(listOne);
        free(listTwo);
    }

    print(aoc, INT, &answer);

    return 0;
}
