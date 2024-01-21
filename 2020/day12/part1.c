/* Day 12, part 1 = 796 */

#include "../../aoc.h"
#include <stdlib.h>

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        enum Direction direction = EAST;
        int rotation = 90;
        int north = 0;
        int east = 0;

        char instruction;
        int value;
        char ch;

        while (fscanf(inputFile, "%c%d%c", &instruction, &value, &ch) >= 2) {
            switch (instruction) {
                case 'N':
                    north += value;
                    break;
                case 'S':
                    north -= value;
                    break;
                case 'E':
                    east += value;
                    break;
                case 'W':
                    east -= value;
                    break;
                case 'L':
                case 'R':
                    for (int i = 0; i < value / 90; i++) {
                        if (instruction == 'L' && rotation == 0) {
                            rotation = 360;
                        } else if (instruction == 'R' && rotation == 360) {
                            rotation = 0;
                        }

                        rotation += 90 * (instruction == 'L' ? -1 : 1);
                    }

                    if (rotation == 360) {
                        rotation = 0;
                    }

                    switch (rotation) {
                        case 0:
                            direction = NORTH;
                            break;
                        case 180:
                            direction = SOUTH;
                            break;
                        case 90:
                            direction = EAST;
                            break;
                        case 270:
                            direction = WEST;
                            break;
                    }
                    break;
                case 'F':
                    switch (direction) {
                        case NORTH:
                            north += value;
                            break;
                        case SOUTH:
                            north -= value;
                            break;
                        case EAST:
                            east += value;
                            break;
                        case WEST:
                            east -= value;
                            break;
                    }
                    break;                    
            }
        }

        fclose(inputFile);

        answer = abs(north) + abs(east);
    }

    print(aoc, INT, &answer);

    return 0;
}
