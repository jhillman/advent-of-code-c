/* Day 8, part 2 = 25007 */

#include "node.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Node *node = readNode(inputFile);
        
        fclose(inputFile);

        answer = nodeValue(node);

        freeNode(node);
    }

    print(aoc, INT, &answer);

    return 0;
}
