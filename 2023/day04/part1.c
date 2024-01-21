/* Day 4, part 1 = 25651 */

#include "../../aoc.h"
#include "cards.h"
#include <math.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cards *cards = getCards(aoc.input);

    if (cards) {
        int answer = 0;

        for (int i = 0; i < cards->count; i++) {
            if (cards->cards[i].winners) {
                answer += (int)pow(2, cards->cards[i].winners - 1);
            }
        }

        freeCards(cards);

        print(aoc, INT, &answer);
    }

    return 0;
}