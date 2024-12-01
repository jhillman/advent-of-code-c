/* Day 13, part 1 = 748 */

#include "../../aoc.h"
#include "layers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Layers *layers = getLayers(aoc.input);
    int answer = 0;

    if (layers) {
        int severity = 0;

        for (int i = 0; i < layers->count; i++) {
            if (caught(layers->data[i], 0)) {
                severity += layers->data[i].severity;
            }
        }

        answer = severity;

        freeLayers(layers);
    }

    print(aoc, INT, &answer);

    return 0;
}
