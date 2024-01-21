/* Day 7, part 1 = 119 */

#include "../../aoc.h"
#include "bags.h"

bool bagCanContain(struct Rule ruleLookUp[WAVY - BRIGHT + 1][YELLOW - AQUA + 1], struct Bag bag, enum Modifier modifier, enum Color color) {
    struct Rule rule = ruleLookUp[bag.modifier][bag.color];
    bool canContain = false;

    if (rule.canContain) {
        canContain = true;
    } else if (bag.modifier == modifier && bag.color == color) {
        ruleLookUp[bag.modifier][bag.color].canContain = 1;

        canContain = true;
    } else if (rule.bagCount > 0) {
        for (int i = 0; !canContain && i < rule.bagCount; i++) {
            canContain = bagCanContain(ruleLookUp, rule.bags[i], modifier, color);
        }
    }

    return canContain;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct RuleData data = readInput(aoc.input);
    int answer = 0;

    if (data.ruleCount > 0) {
        int bagCount = 0;
        enum Modifier modifier = SHINY;
        enum Color color = GOLD;

        for (int i = 0; i < data.ruleCount; i++) {
            struct Rule rule = data.rules[i];

            if (rule.bag.modifier == modifier && rule.bag.color == color) {
                continue;
            }

            if (bagCanContain(data.ruleLookUp, rule.bag, modifier, color)) {
                ++bagCount;
            }
        }

        freeData(data);

        answer = bagCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
