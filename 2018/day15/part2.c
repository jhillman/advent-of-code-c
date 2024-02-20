/* Day 15, part 2 = 53222 */

#include "battle.h"

int bestOutcome(struct Battle *battle, int lowPower, int highPower) {
    int power = lowPower + ((highPower - lowPower) / 2);

    resetBattle(battle);
    setElfAttackPower(battle, power);

    while (!battle->ended) {
        proceed(battle);
    }

    if (allElvesSurvived(battle)) {
        if (power == lowPower + 1) {
            return battle->outcome;
        } else {
            return bestOutcome(battle, lowPower, power - 1);
        }
    } else {
        return bestOutcome(battle, power, highPower);
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Battle *battle = getBattle(aoc.input);
    int answer = 0;

    if (battle) {
        int power = 30;
        int powerDelta = -5;
        bool bestOutcomeFound = false;

        while (!bestOutcomeFound) {
            resetBattle(battle);
            setElfAttackPower(battle, power);

            while (!battle->ended) {
                proceed(battle);
            }
            
            if (allElvesSurvived(battle) && powerDelta == 1) {
                bestOutcomeFound = true;
            } else if (!allElvesSurvived(battle) && powerDelta != 1) {
                powerDelta = 1;
            }

            power += powerDelta;
        }

        answer = battle->outcome;

        freeBattle(battle);
    }

    print(aoc, INT, &answer);

    return 0;
}
