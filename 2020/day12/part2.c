/* Day 12, part 2 = 39446 */

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
        int shipEast = 0;
        int shipNorth = 0;
        int waypointEast = 10;
        int waypointNorth = 1;

        char instruction;
        int value;
        char ch;

        while (fscanf(inputFile, "%c%d%c", &instruction, &value, &ch) >= 2) {
            switch (instruction) {
                case 'N':
                    waypointNorth += value;
                    break;
                case 'S':
                    waypointNorth -= value;
                    break;
                case 'E':
                    waypointEast += value;
                    break;
                case 'W':
                    waypointEast -= value;
                    break;
                case 'L':
                    for (int i = 0; i < value / 90; i++) {
                        int temp = waypointEast;
                        waypointEast = -waypointNorth;
                        waypointNorth = temp;
                    }
                    break;
                case 'R':
                    for (int i = 0; i < value / 90; i++) {
                        int temp = waypointEast;
                        waypointEast = waypointNorth;
                        waypointNorth = -temp;
                    }
                    break;
                case 'F':
                    shipEast += (value * waypointEast);
                    shipNorth += (value * waypointNorth);
                    break;                    
            }
        }

        fclose(inputFile);

        answer = abs(shipNorth) + abs(shipEast);
    }

    print(aoc, INT, &answer);

    return 0;
}
