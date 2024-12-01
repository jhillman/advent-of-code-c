/* Day 12, part 1 = 115 */

#include "../../aoc.h"
#include "nodes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Nodes *nodes = getNodes(aoc.input);
    int answer = 0;

    if (nodes) {
        bool *visited = (bool *)calloc(2000, sizeof(bool));

        answer = connectedCount(nodes, 0, visited);

        free(visited);
        freeNodes(nodes);
    }

    print(aoc, INT, &answer);

    return 0;
}
