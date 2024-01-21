/* Day 13, part 1 = 156 */

#include "../../aoc.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int timestamp;

        fscanf(inputFile, "%d", &timestamp);

        char busSchedule[200];
        int minWaitTime = -1;

        if (fscanf(inputFile, "%s", busSchedule)) {
            char *busId = strtok(busSchedule, ",");

            while (busId) {
                if (*busId != 'x') {
                    int busDeparture = atoi(busId);

                    int busTime = busDeparture;

                    while (busTime < timestamp) {
                        busTime += busDeparture;
                    }

                    int waitTime = busDeparture * (busTime - timestamp);

                    if (minWaitTime == -1 || waitTime < minWaitTime) {
                        minWaitTime = waitTime;
                    }
                }

                busId = strtok(NULL, ",");
            }
        }

        fclose(inputFile);

        answer = minWaitTime;
    }

    print(aoc, INT, &answer);

    return 0;
}
