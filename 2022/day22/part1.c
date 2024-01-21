/* Day 22, part 1 = 64256 */

#include "../../aoc.h"
#include "map.h"

void handleMove(int moveAmount, char **map, int width, int height, int cubeWidth, char *facing, int *x, int *y) {
    for (int i = 0; i < moveAmount; i++) {
        map[*y][*x] = *facing;

        int lastX = *x;
        int lastY = *y;

        switch (*facing) {
        case '^':
            --*y;

            if (*y < 0 || map[*y][*x] == ' ') {
                *y = height - 1;

                while (map[*y][*x] == ' ') {
                    --*y;
                }
            }
            break;
        case '<':
            --*x;

            if (*x < 0 || map[*y][*x] == ' ') {
                *x = width - 1;

                while (map[*y][*x] == ' ') {
                    --*x;
                }
            }
            break;
        case '>':
            ++*x;

            if (*x > width - 1 || map[*y][*x] == ' ') {
                int lastX = --*x;

                *x = 0;

                while (map[*y][*x] == ' ') {
                    ++*x;
                }
            }
            break;
        case 'v':
            ++*y;

            if (*y > height - 1 || map[*y][*x] == ' ') {
                int lastY = --*y;

                *y = 0;

                while (map[*y][*x] == ' ') {
                    ++*y;
                }
            }
            break;
        }

        if (map[*y][*x] == '#') {
            *x = lastX;
            *y = lastY;
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
