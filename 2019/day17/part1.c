/* Day 17, part 1 = 10632 */

#include "../../aoc.h"
#include "scaffold.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    
    struct Scaffold *scaffold = getScaffold(aoc.input);
    int answer = 0;

    if (scaffold) {
        int xDeltas[] = {-1, 0, 1, 0};
        int yDeltas[] = {0, 1, 0, -1};
        int alignmentParameterSum = 0;
  
        for (int y = 0; y < scaffold->height; y++) {
            for (int x = 0; x < scaffold->width; x++) {
                if (scaffold->grid[y][x] == '#') {
                    bool intersection = true;

                    for (int i = 0; intersection && i < 4; i++) {
                        int dx = x + xDeltas[i];
                        int dy = y + yDeltas[i];

                        if (!(dx >= 0 && dx < scaffold->width && dy >= 0 && dy < scaffold->height)) {
                            intersection = false;
                            break;
                        }

                        intersection = scaffold->grid[dy][dx] == '#';
                    }

                    if (intersection) {
                        alignmentParameterSum += x * y;
                    }
                }
            }
        }

        answer = alignmentParameterSum;

        freeScaffold(scaffold);
    }

    print(aoc, INT, &answer);

    return 0;
}
