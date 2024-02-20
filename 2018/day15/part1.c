/* Day 15, part 1 = 214731 */

#include "battle.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Battle *battle = getBattle(aoc.input);
    int answer = 0;

    if (battle) {
        while (!battle->ended) {
            proceed(battle);
        }

        answer = battle->outcome;

        freeBattle(battle);
    }

    print(aoc, INT, &answer);

    return 0;
}