/* Day 16, part 1 = 1038 */

#include "../../aoc.h"
#include "bits.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    char *transmission = getTransmission(aoc.input);
    long answer = 0;

    if (transmission) {
        char *packet = transmission;
        struct ExpressionNode *expression = (struct ExpressionNode *)calloc(1, sizeof(struct ExpressionNode));

        readPacket(packet, NULL, expression);

        free(transmission);

        answer = versionSum(expression);

        freeExpression(expression);
    }

    print(aoc, LONG, &answer);

    return 0;
}
