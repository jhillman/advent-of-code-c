/* Day 24, part 1 = 29989297949519 */

#include "../../aoc.h"
#include "commands.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CommandData *data = getCommandData(aoc.input);
    long answer = 0;

    if (data) {
        answer = findModelNumber(data, true);
    }

    freeCommandData(data);

    print(aoc, LONG, &answer);
}
