/* Day 15, part 2 = 2821 */

#include "../../aoc.h"
#include "chitons.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ChitonsData *data = getChitonsData(aoc.input, 5);
    int answer = 0;

    if (data) {
        int originalWidth = data->width / 5;
        int originalHeight = data->height / 5;

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (y >= originalHeight || x >= originalWidth) {
                    int level = y >= originalHeight ? 
                        data->riskLevels[y - originalHeight][x] : 
                        data->riskLevels[y][x - originalWidth];

                    if (++level > 9) {
                        level = 1;
                    }

                    data->riskLevels[y][x] = level;
                }
            }
        }

        answer = lowestRisk(data);

        freeChitonsData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
