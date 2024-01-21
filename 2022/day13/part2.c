/* Day 13, part 2 = 24477 */

#include "../../aoc.h"
#include "packets.h"

int sort(const void *a, const void *b) {
    enum ComparisonResult result = compare(*(struct ListItem *)a, *(struct ListItem *)b);

    return result == LEFT ? -1 : (result == RIGHT ? 1 : 0);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int packetCount;
    struct ListItem *packets = getPackets(aoc.input, &packetCount);
    int answer = 0;

    if (packets) {
        packets = (struct ListItem *)realloc(packets, (packetCount + 2) * sizeof(struct ListItem));

        packets[packetCount++] = readList("[[2]]", NULL);
        packets[packetCount - 1].divider = true;
        packets[packetCount++] = readList("[[6]]", NULL);
        packets[packetCount - 1].divider = true;

        qsort(packets, packetCount, sizeof(struct ListItem), sort);

        int decoderKey = 1;

        for (int i = 0; i < packetCount; i++) {
            if (packets[i].divider) {
                decoderKey *= (i + 1);
            }
        }

        freeList(packets);

        answer = decoderKey;
    }

    print(aoc, INT, &answer);

    return 0;
}
