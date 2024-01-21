/* Day 19, part 1 = 459 */

#include "../../aoc.h"
#include "scanners.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ScannerData *data = getScannerData(aoc.input);
    int answer = 0;

    if (data) {
        mapScanners(data);

        answer = data->beacons->size;

        freeScannerData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
