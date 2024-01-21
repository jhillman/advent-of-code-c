/* Day 5, part 1 = 5280 */

#include "../../aoc.h"
#include "diagram.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct DiagramData *data = getDiagramData(aoc.input);
    int answer = 0;

    if (data) {
        answer = updateDiagram(data, false);

        freeDiagramData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
