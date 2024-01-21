/* Day 22, part 2 = 33661 */

#include "../../aoc.h"
#include "cards.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct GameData *data = readGameData(aoc.input);
    int answer = 0;

    if (data) {
        if (playGame(data->playerOneDeck, data->playerTwoDeck, true) == 1) {
            answer = deckScore(data->playerOneDeck);
        } else {
            answer = deckScore(data->playerTwoDeck);
        }

        freeDeck(data->playerOneDeck);
        freeDeck(data->playerTwoDeck);
        free(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
