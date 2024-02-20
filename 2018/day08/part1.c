/* Day 8, part 1 = 42254 */

#include "node.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Node *node = readNode(inputFile);
        
        fclose(inputFile);

        answer = metadataSum(node);

        freeNode(node);
    }

    print(aoc, INT, &answer);

    return 0;
}
