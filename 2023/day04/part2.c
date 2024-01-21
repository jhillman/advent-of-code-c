/* Day 4, part 2 = 19499881 */

#include "../../aoc.h"
#include "cards.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cards *cards = getCards(aoc.input);

    if (cards) {
        int answer = 0;

        for (int i = 0; i < cards->count; i++) {
            for (int j = i + 1; j < cards->count && j < i + cards->cards[i].winners + 1; j++) {
                cards->cards[j].count += cards->cards[i].count;
            }
        }

        for (int i = 0; i < cards->count; i++) {
            answer += cards->cards[i].count;
        }

        freeCards(cards);

        print(aoc, INT, &answer);
    }

    return 0;
}
