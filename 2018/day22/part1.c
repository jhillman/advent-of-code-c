/* Day 22, part 1 = 4479 */

#include "cave.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cave *cave = getCave(aoc.input);
    int answer = 0;

    if (cave) {
        int riskLevel = 0;

        for (int y = 0; y < cave->height; y++) {
            for (int x = 0; x < cave->width; x++) {
                riskLevel += regionType(cave, (struct Location){ x, y });
            }
        }

        answer = riskLevel;

        freeCave(cave);
    }

    print(aoc, INT, &answer);

    return 0;
}
