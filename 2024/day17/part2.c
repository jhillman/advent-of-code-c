/* Day 17, part 2 = 202356708354602 */

#include "../../aoc.h"
#include "computer.h"

bool evaluate(long output) {
    return true;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Computer *computer = getComputer(aoc.input);
    long answer = 0;

    if (computer) {
        long a = 0;
        long additional;
        long out = 0;
        int programDigit = computer->programLength - 1;
        struct Output output = { NULL, &out, evaluate };

        while (programDigit >= 0) {
            a *= 8;
            additional = 0;

            while (true) {
                computer->instruction = 0;
                computer->a = a + additional;
                computer->b = 0;
                computer->c = 0;
                *output.integer = 0;

                run(computer, &output);

                if (*output.integer == computer->program[programDigit]) {
                    a += additional;
        
                    --programDigit;
                    break;
                }

                ++additional;
            }
        }

        answer = a;

        freeComputer(computer);
    }

    print(aoc, LONG, &answer);

    return 0;
}