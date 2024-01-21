/* Day 22, part 2 = 109224 */

#include "../../aoc.h"
#include "map.h"

void handleMove(int moveAmount, char **map, int width, int height, int cubeWidth, char *facing, int *x, int *y) {
    for (int i = 0; i < moveAmount; i++) {
        map[*y][*x] = *facing;

        int lastX = *x;
        int lastY = *y;
        char lastFacing = *facing;

        switch (*facing) {
        case '^':
            --*y;

            if (*y < 0 || map[*y][*x] == ' ') {
                if (*x < cubeWidth) {
                    *x = cubeWidth;
                    *y = cubeWidth + lastX;
                    *facing = '>';
                } else if (*x < cubeWidth * 2) {
                    *x = 0;
                    *y = (cubeWidth * 2) + lastX;
                    *facing = '>';
                } else {
                    *x = lastX - (cubeWidth * 2);
                    *y = (cubeWidth * 4) - 1;
                    *facing = '^';
                }
            }
            break;
        case '<':
            --*x;

            if (*x < 0 || map[*y][*x] == ' ') {
                if (*y < cubeWidth) {
                    *x = 0;
                    *y = (cubeWidth * 3) - lastY - 1;
                    *facing = '>';
                } else if (*y < cubeWidth * 2) {
                    *x = lastY - cubeWidth;
                    *y = (cubeWidth * 2) - 1;
                    *facing = 'v';
                } else if (*y < cubeWidth * 3) {
                    *x = cubeWidth;
                    *y = lastY - (cubeWidth * 2);
                    *facing = '>';
                } else {
                    *x = lastY - (cubeWidth * 2);
                    *y = 0;
                    *facing = 'v';
                }
            }
            break;
        case '>':
            ++*x;

            if (*x > width - 1 || map[*y][*x] == ' ') {
                if (*y < cubeWidth) {
                    *x = (cubeWidth * 2) - 1;
                    *y = (cubeWidth * 3) - lastY;
                    *facing = '<';
                } else if (*y < cubeWidth * 2) {
                    *x = lastY + cubeWidth;
                    *y = cubeWidth - 1;
                    *facing = '^';
                } else if (*y < cubeWidth * 3) {
                    *x = (cubeWidth * 3) - 1;
                    *y = (cubeWidth * 3) - lastY;
                    *facing = '<';
                } else {
                    *x = lastY - (cubeWidth * 2);
                    *y = (cubeWidth * 3) - 1;
                    *facing = '^';
                }
            }
            break;
        case 'v':
            ++*y;

            if (*y > height - 1 || map[*y][*x] == ' ') {
                if (*x < cubeWidth) {
                    *x = lastX + (cubeWidth * 2);
                    *y = 0;
                    *facing = 'v';
                } else if (*x < cubeWidth * 2) {
                    *x = cubeWidth - 1;
                    *y = lastX + (cubeWidth * 2);
                    *facing = '<';
                } else {
                    *x = (cubeWidth * 2) - 1;
                    *y = lastX - cubeWidth;
                    *facing = '<';
                }
            }
            break;
        }

        if (map[*y][*x] == '#') {
            *x = lastX;
            *y = lastY;
            *facing = lastFacing;
            break;
        }
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = navigateMap(aoc.input);

    print(aoc, INT, &answer);

    return 0;
}
