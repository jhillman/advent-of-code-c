/* Day 13, part 2 = 3873662 */

#include "../../aoc.h"
#include "layers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Layers *layers = getLayers(aoc.input);
    int answer = 0;

    if (layers) {
        int step = 0;
        bool successful = false;

        while (!successful) {
            successful = true;
            ++step;

            for (int i = 0; successful && i < layers->count; i++) {
                successful = !caught(layers->data[i], step);
            }
        }

        answer = step;

        freeLayers(layers);
    }

    print(aoc, INT, &answer);

    return 0;
}
