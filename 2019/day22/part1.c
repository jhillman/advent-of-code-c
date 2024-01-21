/* Day 22, part 1 = 6696 */

#include "../../aoc.h"
#include <string.h>

#define CARD_COUNT 10007

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int deck[CARD_COUNT];
    int buffer[CARD_COUNT];

    for (int card = 0; card < CARD_COUNT; card++) {
        deck[card] = card;
    }

    char line[32];
    int value;
    int *top;
    int *bottom;
    int cut;
    int count;
    int answer = 0;

    if (inputFile) {
        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            if (strcmp(line, "deal into new stack") == 0) {
                top = deck;
                bottom = deck + CARD_COUNT - 1;

                while (top < bottom) {
                    value = *bottom;
                    *bottom-- = *top;
                    *top++ = value;
                }
            } else if (sscanf(line, "cut %d", &value)) {
                cut = value < 0 ? CARD_COUNT + value : value;
                count = CARD_COUNT - cut;

                memcpy(buffer, deck, cut * sizeof(int));
                memcpy(deck, deck + cut, count * sizeof(int));
                memcpy(deck + count, buffer, cut * sizeof(int));
            } else if (sscanf(line, "deal with increment %d", &value)) {
                memcpy(buffer, deck, CARD_COUNT * sizeof(int));

                for (int offset = 0; offset < CARD_COUNT; offset++) {
                    deck[(offset * value) % CARD_COUNT] = buffer[offset];
                }
            }
        }

        fclose(inputFile);
    }

    for (int offset = 0; !answer && offset < CARD_COUNT; offset++) {
        if (deck[offset] == 2019) {
            answer = offset;
        }
    }
    
    print(aoc, INT, &answer);

    return 0;
}
