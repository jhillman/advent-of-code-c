/* Day 8, part 1 = 521 */

#include "../../aoc.h"
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char value[8];
        char ch;

        while (fscanf(inputFile, "%s%c", value, &ch) >= 1) {
            if (strcmp(value, "|") == 0) {
                for (int i = 0; i < 4; i++) {
                    fscanf(inputFile, "%s%c", value, &ch);
                    
                    int outputLength = strlen(value);

                    switch (strlen(value)) {
                        case 2:
                        case 3:
                        case 4:
                        case 7:
                            ++answer;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
