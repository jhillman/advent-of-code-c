/* Day 23, part 2 = 13334 */

#include "../../aoc.h"
#include "network.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = networkPacket(aoc.input, true);
    
    print(aoc, INT, &answer);

    return 0;
}
