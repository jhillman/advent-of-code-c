/* Day 23, part 1 = 27956483 */

#include "../../aoc.h"
#include "cups.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CupData *data = readCupData(aoc.input);
    char answer[16] = { 0 };

    if (data) {
        struct Cup *cup = data->first;

        for (int i = 0; i < 100; i++) {
            cup = move(cup, data->cupLookup, data->cupCount);
        }

        cup = data->cupLookup[1]->next;

        char *label = answer;

        do {
            *label++ = cup->label + '0';
            *label = '\0';

            cup = cup->next;
        } while (cup->label != 1);

        freeCupData(data);
    }

    print(aoc, STRING, answer);

    return 0;
}
