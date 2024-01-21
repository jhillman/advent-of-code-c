/* Day 16, part 1 = 27898 */

#include "../../aoc.h"
#include "tickets.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct TicketData *data = getTicketData(aoc.input);
    int answer = 0;

    if (data) {
        int invalidSum = 0;

        for (int i = 0; i < data->nearbyCount; i++) {
            for (int j = 0; j < data->valueCount; j++) {
                if (!data->nearby[i].values[j].valid) {
                    invalidSum += data->nearby[i].values[j].value;
                }
            }
        }

        freeTicketData(data);

        answer = invalidSum;
    }

    print(aoc, INT, &answer);

    return 0;
}
