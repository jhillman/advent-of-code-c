/* Day 17, part 1 = 7,1,2,3,2,6,7,2,5 */

#include "../../aoc.h"
#include "computer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Computer *computer = getComputer(aoc.input);
    char answer[32] = "";

    if (computer) {
        struct Output output = { answer, NULL };
        
        run(computer, &output);

        freeComputer(computer);
    }

    print(aoc, STRING, &answer);

    return 0;
}
