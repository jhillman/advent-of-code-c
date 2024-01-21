/* Day 5, part 2 = 16716 */

#include "../../aoc.h"
#include "diagram.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct DiagramData *data = getDiagramData(aoc.input);
    int answer = 0;

    if (data) {
        answer = updateDiagram(data, true);

        freeDiagramData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
