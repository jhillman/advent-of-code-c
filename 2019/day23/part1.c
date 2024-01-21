/* Day 23, part 1 = 20372 */

#include "../../aoc.h"
#include "network.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = networkPacket(aoc.input, false);
    
    print(aoc, INT, &answer);

    return 0;
}
