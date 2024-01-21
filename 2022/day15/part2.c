/* Day 15, part 2 = 13029714573243 */

#include "../../aoc.h"
#include "sensors.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ScanData *data = getScanData(aoc.input);
    long answer = 0;

    if (data) {
        int maxCoordinate = 4000000;
        long x = 0;
        long y = 0;
        bool beaconFound = false;

        while (!beaconFound && y <= maxCoordinate) {
            x = 0;

            while (!beaconFound && x <= maxCoordinate) {
                struct Sensor *sensor = sensorForXY(data->sensors, data->sensorCount, x, y);
    
                if (sensor) {
                    x = maxXForY(*sensor, y) + 1;
                } else {
                    beaconFound = true;
                }
            }

            if (!beaconFound) {
                ++y;
            }
        }

        freeScanData(data);

        answer = x * maxCoordinate + y;
    }

    print(aoc, LONG, &answer);

    return 0;
}
