/* Day 19, part 2 = 19130 */

#include "../../aoc.h"
#include "scanners.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ScannerData *data = getScannerData(aoc.input);
    int answer = 0;

    if (data) {
        mapScanners(data);

        for (int i = 0; i < data->scannerCount; i++) {
            for (int j = i + 1; j < data->scannerCount; j++) {
                int scannerDistance = distance(data->scanners[i]->translation, data->scanners[j]->translation);

                if (scannerDistance > answer) {
                    answer = scannerDistance;
                }
            }
        }

        freeScannerData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
