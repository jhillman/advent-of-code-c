#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int digitCount(long number) {
    return (int)floor(log10((double)number)) + 1;
}

int readNumbers(char *position, long **array, bool fixInputKerning) {
    int count = 0;
    long number;

    while (*position) {
        while (*position && !isdigit(*position)) {
            ++position;
        }

        if (*position) {
            sscanf(position, "%ld", &number);

            if (fixInputKerning && count > 0) {
                **array = (**array * pow(10, digitCount(number))) + number;
            } else {
                *array = (long *)realloc(*array, (count + 1) * sizeof(long));
                *(*array + count++) = number;
            }

            position += digitCount(number);
        }
    }

    return count;
}

long getRaceAnswer(char *input, bool fixInputKerning) {
    FILE *inputFile = fopen(input, "r");
    long answer = 1;

    if (inputFile) {
        char line[64];
        char *position;
        int raceCount;
        long *times = NULL;
        long *distances = NULL;

        fgets(line, sizeof(line), inputFile);
        line[strcspn(line, "\n")] = 0;

        position = line;

        raceCount = readNumbers(position, &times, fixInputKerning);

        fgets(line, sizeof(line), inputFile);
        line[strcspn(line, "\n")] = 0;

        position = line;

        readNumbers(position, &distances, fixInputKerning);

        for (int i = 0; i < raceCount; i++) {
            long totalTime = times[i];
            long record = distances[i];
            long startTime = 0;
            long endTime = 0;
            long timeDelta = pow(10, digitCount(totalTime) - 2);
            bool narrowing = false;

            for (long time = 0; !startTime && time < times[i]; time += timeDelta) {
                if (time * (totalTime - time) > record) {
                    if (narrowing) {
                        startTime = time;
                    } else {
                        narrowing = true;
                        time -= timeDelta;
                        timeDelta = 1;
                    }
                }
            }

            narrowing = false;
            timeDelta = pow(10, digitCount(totalTime) - 2);

            for (long time = times[i]; !endTime && time >= 0; time -= timeDelta) {
                if (time * (totalTime - time) > record) {
                    if (narrowing) {
                        endTime = time;
                    } else {
                        narrowing = true;
                        time += timeDelta;
                        timeDelta = 1;
                    }
                }
            }

            answer *= (endTime - (startTime - 1));
        }

        free(times);
        free(distances);

        fclose(inputFile);
    }

    return answer;
}
