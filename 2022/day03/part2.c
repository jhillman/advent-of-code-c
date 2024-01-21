/* Day 3, part 2 = 2650 */

#include "../../aoc.h"
#include <string.h>
#include "priority.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char rucksack1[64];
        char rucksack2[64];
        char rucksack3[64];
        char *item;
        int prioritySum = 0;

        while (fgets(rucksack1, sizeof(rucksack1), inputFile) && 
               fgets(rucksack2, sizeof(rucksack2), inputFile) && 
               fgets(rucksack3, sizeof(rucksack3), inputFile)) {
            item = rucksack1;

            while (item) {
                if (strchr(rucksack2, *item) && strchr(rucksack3, *item)) {
                    prioritySum += priority(*item);
                    break;
                }

                ++item;
            }
        }

        fclose(inputFile);

        answer = prioritySum;
    }

    print(aoc, INT, &answer);

    return 0;
}
