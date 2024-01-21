/* Day 15, part 1 = 4951427 */

#include "../../aoc.h"
#include "sensors.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ScanData *data = getScanData(aoc.input);
    int answer = 0;

    if (data) {
        int targetY = 2000000;
        int minX = 10000000;
        int maxX = 0;

        for (int i = 0; i < data->sensorCount; i++) {
            minX = min(minX, minXForY(data->sensors[i], targetY));
            maxX = max(maxX, maxXForY(data->sensors[i], targetY));
        }

        int positionCount = 0;
        int x = minX;

        while (x <= maxX) {
            struct Sensor *sensor = sensorForXY(data->sensors, data->sensorCount, x, targetY);

            if (sensor) {
                int maxSensorX = maxXForY(*sensor, targetY);

                positionCount += maxSensorX - x + 1;

                x = maxSensorX;
            }

            ++x;
        }

        for (int i = 0; i < data->beaconCount; i++) {
            if (data->beacons[i].y == targetY && data->beacons[i].x >= minX && data->beacons[i].x <= maxX) {
                --positionCount;
            }
        }

        freeScanData(data);

        answer = positionCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
