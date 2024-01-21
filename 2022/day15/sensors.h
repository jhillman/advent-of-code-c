#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

struct Beacon {
    int x;
    int y;
};

struct Sensor {
    int i;
    int x;
    int y;
    struct Beacon beacon;
    int beaconDistance;
};

int minXForY(struct Sensor sensor, int y) {
    return sensor.x - abs(sensor.beaconDistance - abs(y - sensor.y));
}

int maxXForY(struct Sensor sensor, int y) {
    return sensor.x + abs(sensor.beaconDistance - abs(y - sensor.y));
}

struct Sensor *sensorForXY(struct Sensor *sensors, int sensorCount, int x, int y) {
    for (int i = 0; i < sensorCount; i++) {
        if (distance(sensors[i].x, sensors[i].y, x, y) <= sensors[i].beaconDistance) {
            return &sensors[i];
        }
    }

    return NULL;
}

struct ScanData {
    struct Sensor *sensors;
    int sensorCount;
    struct Beacon *beacons;
    int beaconCount;
};

struct ScanData *getScanData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[128];

        struct ScanData *data = (struct ScanData *)calloc(1, sizeof(struct ScanData));

        while (fgets(line, sizeof(line), inputFile)) {
            ++data->sensorCount;
        }

        data->sensors = (struct Sensor *)malloc(data->sensorCount * sizeof(struct Sensor));

        fseek(inputFile, 0, SEEK_SET);

        for (int i = 0; i < data->sensorCount; i++) {
            fgets(line, sizeof(line), inputFile);

            sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", 
                &data->sensors[i].x, &data->sensors[i].y, &data->sensors[i].beacon.x, &data->sensors[i].beacon.y);

            bool beaconFound = false;

            for (int j = 0; !beaconFound && j < data->beaconCount; j++) {
                if (data->beacons[j].x == data->sensors[i].beacon.x && data->beacons[j].y == data->sensors[i].beacon.y) {
                    beaconFound = true;
                }
            }

            if (!beaconFound) {
                if (data->beacons) {
                    data->beacons = (struct Beacon *)realloc(data->beacons, (data->beaconCount + 1) * sizeof(struct Beacon));
                } else {
                    data->beacons = (struct Beacon *)malloc((data->beaconCount + 1) * sizeof(struct Beacon));
                }
    
                data->beacons[data->beaconCount++] = data->sensors[i].beacon;
            }

            data->sensors[i].beaconDistance = distance(data->sensors[i].x, data->sensors[i].y, data->sensors[i].beacon.x, data->sensors[i].beacon.y);
        }        

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeScanData(struct ScanData *data) {
    free(data->sensors);
    free(data->beacons);

    free(data);
}