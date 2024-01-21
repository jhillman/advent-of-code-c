/* Day 13, part 1 = 5825 */

#include "../../aoc.h"
#include "packets.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int packetCount;
    struct ListItem *packets = getPackets(aoc.input, &packetCount);
    int answer = 0;

    if (packets) {
        int indicesSum = 0;
        int index = 0;

        for (int i = 0; i < packetCount; i += 2) {
            ++index;

            if (compare(packets[i], packets[i + 1]) == LEFT) {
                indicesSum += index;
            }
        }

        freeList(packets);

        answer = indicesSum;
    }

    print(aoc, INT, &answer);

    return 0;
}
