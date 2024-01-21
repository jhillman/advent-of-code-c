/* Day 19, part 2 = 392 */

#include "../../aoc.h"
#include "match.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct RulesData *data = readRulesData(aoc.input);
    int answer = 0;

    if (data) {
        char ruleString[20];

        strcpy(ruleString, "42 | 42 8");
        parseRule(8, ruleString, data->rules);

        strcpy(ruleString, "42 31 | 42 11 31");
        parseRule(11, ruleString, data->rules);

        int validCount = validInputCount(data);

        freeRulesData(data);

        answer = validCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
