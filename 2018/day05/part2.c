/* Day 5, part 2 = 6394 */

#include "polymer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Polymer *polymer = getReactedPolymer(aoc.input);
    int answer = 0;

    if (polymer) {
        int minimumLength = polymer->length;
        struct Lefts *lefts = (struct Lefts *)calloc(1, sizeof(struct Lefts));

        for (char unit = 'a'; unit <= 'z'; unit++) {
            char opposite = oppositePolarity(unit);
            int letterLength = polymer->length;
            
            lefts->count = 0;

            for (int i = 0; i < polymer->length; i++) {
                if (polymer->units[i] == unit || polymer->units[i] == opposite) {
                    --letterLength;

                    while (i < polymer->length && polymer->units[i + 1] == unit || polymer->units[i + 1] == opposite) {
                        --letterLength;
                        ++i;
                    }

                    int right = i + 1;

                    while (lefts->count > 0 && right < polymer->length && rightmostLeftUnit(lefts) == oppositePolarity(polymer->units[right])) {
                        letterLength -= 2;
                        --lefts->count;
                        ++right;
                    }

                    i = right - 1;
                } else {
                    addLeftUnit(lefts, polymer->units[i]);
                }
            }

            if (letterLength < minimumLength) {
                minimumLength = letterLength;
            }
        }

        answer = minimumLength;

        free(lefts->data);
        free(lefts);

        freePolymer(polymer);
    }

    print(aoc, INT, &answer);

    return 0;
}
