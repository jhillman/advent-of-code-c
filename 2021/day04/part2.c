/* Day 4, part 2 = 8224 */

#include "../../aoc.h"
#include "bingo.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct BingoData *data = getBingoData(aoc.input);
    int answer = 0;

    if (data) {
        int losingBoards = data->boardCount;

        for (int i = 0; !answer && i < data->numberCount; i++) {
            int number = data->numbers[i];

            for (int j = 0; !answer && j < data->boardCount; j++) {
                playNumber(&data->boards[j], number);

                if (!data->boards[j].winner && boardWins(&data->boards[j])) {
                    if (--losingBoards == 0) {
                        answer = number * data->boards[j].unmarkedSum;
                    }
                }
            }
        }

        freeBingoData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
