/* Day 10, part 1 = 230 */

#include "../../aoc.h"
#include "astroids.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int astroidCount;
    struct Astroid *astroids = getAstroids(aoc.input, &astroidCount);
    int answer = 0;

    if (astroids) {
        int maxVisible = 0;

        for (int i = 0; i < astroidCount; i++) {
            int visibleCount;
            struct AstroidData *visibleAstroids = astroidData(astroids[i], astroids, astroidCount, true, &visibleCount);

            if (visibleCount > maxVisible) {
                maxVisible = visibleCount;
            }

            free(visibleAstroids);
        }

        free(astroids);

        answer = maxVisible;
    }

    print(aoc, INT, &answer);

    return 0;
}
