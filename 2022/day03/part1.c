/* Day 3, part 1 = 7428 */

#include "../../aoc.h"
#include <string.h>
#include "priority.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char rucksack[64];
        char compartment1[32];
        char compartment2[32];
        char *item;
        int prioritySum = 0;

        while (fgets(rucksack, sizeof(rucksack), inputFile)) {
            int compartmentSize = strlen(rucksack) / 2;
            *compartment1 = *compartment2 = '\0';

            strncat(compartment1, rucksack, compartmentSize);
            strncat(compartment2, rucksack + compartmentSize, compartmentSize);

            item = compartment1;

            while (item) {
                if (strchr(compartment2, *item)) {
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
