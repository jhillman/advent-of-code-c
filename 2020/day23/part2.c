/* Day 23, part 2 = 18930983775 */

#include "../../aoc.h"
#include "cups.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CupData *data = readCupData(aoc.input);
    long answer = 0;

    if (data) {
        struct Cup *cup;
        struct Cup *previous = data->first->previous;
        
        data->cupLookup = (struct Cup **) realloc(data->cupLookup, 1000001 * sizeof(struct Cup *));
        data->cupCount = 1000000;

        for (int label = 10; label <= 1000000; label++) {
            cup = (struct Cup *) malloc(sizeof(struct Cup));
            cup->label = label;

            data->cupLookup[cup->label] = cup;

            cup->previous = previous;
            previous->next = cup;

            previous = cup;
        }

        cup->next = data->first;
        data->first->previous = cup;

        cup = data->first;

        for (int i = 0; i < 10000000; i++) {
            cup = move(cup, data->cupLookup, data->cupCount);
        }

        long label1 = data->cupLookup[1]->next->label;
        long label2 = data->cupLookup[1]->next->next->label;

        answer = label1 * label2;

        freeCupData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
