/* Day 10, part 2 = EFUGLPAP */

#include "../../aoc.h"
#include "cpu.h"
#include "stdlib.h"

#define PIXELS_WIDTH 40

void handleCycle(int cycle, int x, void *output) {
    char *pixels = (char *)output;

    int position = (cycle % PIXELS_WIDTH) - 1;

    if (position == -1) {
        // printf("%s\n", pixels);
        memset(pixels, '.', PIXELS_WIDTH);
    }

    if (abs(position - x) <= 1 && position >= 0) {
        pixels[position] = '#';
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    char pixels[PIXELS_WIDTH + 1] = { 0 };
    memset(pixels, '.', PIXELS_WIDTH);

    runCpu(aoc.input, &pixels);

    print(aoc, STRING, "EFUGLPAP");

    return 0;
}
