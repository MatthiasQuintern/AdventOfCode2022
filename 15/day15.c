#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SENSORS 30
#define MAX_X 4000000
#define MAX_Y 4000000

struct Sensor {
    int x, y;
    unsigned range;
};
struct Beacon {
    int x, y;
};
struct Area {
    int left, right, top, bottom;
};

unsigned distance(int x1, int  y1, int x2, int y2) {
    int x_dist = abs(x1 - x2);
    int y_dist = abs(y1 - y2);
    return x_dist + y_dist;
}
int max(int a, int b) {
    if (a > b) { return a; }
    return b;
}
int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int isPositionImpossible(int x, int y, struct Beacon* beacons, struct Sensor* sensors, unsigned sensorCount, unsigned* blockingSensor) {
    for (unsigned i = 0; i < sensorCount; i++) {
        if (x == beacons[i].x && y == beacons[i].y) { continue; }  // if beacon
        if (distance(x, y, sensors[i].x, sensors[i].y) <= sensors[i].range) {
            *blockingSensor = i;
            return 1;
        }
    }
    return 0;
}

int main(int argc, const char** argv) {
    const char* filename = NULL;
    if (argc != 2) {
        filename = "beacons.txt";
    }
    else {
        filename = argv[1];
    }
    FILE* p_file;
    p_file = fopen(filename, "r");
    if (p_file == NULL) {
        printf("Could not open file\n");
        exit(1);
    }
    struct Sensor sensors[MAX_SENSORS];
    struct Beacon beacons[MAX_SENSORS];
    unsigned sensorCount = 0;
    struct Area area;

    char* p_linebuffer = NULL;
    size_t linebufferLength = 0;
    ssize_t lineLength = getline(&p_linebuffer, &linebufferLength, p_file);

    while (lineLength > 0) {
        sscanf(p_linebuffer, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensors[sensorCount].x, &sensors[sensorCount].y, &beacons[sensorCount].x, &beacons[sensorCount].y);
        sensors[sensorCount].range = distance(sensors[sensorCount].x, sensors[sensorCount].y, beacons[sensorCount].x, beacons[sensorCount].y);
        lineLength = getline(&p_linebuffer, &linebufferLength, p_file);
        sensorCount++;
    }
    free(p_linebuffer);
    p_linebuffer = NULL;
    fclose(p_file);

    area.left   = sensors[0].x - sensors[0].range;
    area.right  = sensors[0].x + sensors[0].range;
    area.top    = sensors[0].y - sensors[0].range;
    area.bottom = sensors[0].y + sensors[0].range;
    printf("Sensor: x0%d, y=%d, range=%d\n", sensors[0].x, sensors[0].y, sensors[0].range);
    for (unsigned i = 1; i < sensorCount; i++) {
        area.left   = min(sensors[i].x - sensors[i].range, area.left);
        area.right  = max(sensors[i].x + sensors[i].range, area.right);
        area.top    = min(sensors[i].y - sensors[i].range, area.top);
        area.bottom = max(sensors[i].y + sensors[i].range, area.bottom);
        printf("Sensor: x=%d, y=%d, range=%d\n", sensors[i].x, sensors[i].y, sensors[i].range);
    }
    printf("Area: l-r: %d-%d, t-b: %d-%d\n", area.left, area.right, area.top, area.bottom);
    int no_beacons_for_sure_mate = 0;
    /* int y = 10; */
    int y = 2000000;
    // this could be optmized 50000000%
    unsigned blockingSensor;
    for (int x = area.left; x <= area.right; x++) {
        no_beacons_for_sure_mate += isPositionImpossible(x, y, beacons, sensors, sensorCount, &blockingSensor);
    }
    printf("In row %d, there are %d positions where no beacons are possible\n", y, no_beacons_for_sure_mate);

    for (int x = 0; x < MAX_X; x++) {
        for (int y = 0; y < MAX_Y; y++) {
            if (!isPositionImpossible(x, y, beacons, sensors, sensorCount, &blockingSensor)) {
                unsigned long frequency = (unsigned long) x * 4000000 + y;
                printf("Tuning frequency from beacon at (%d, %d): %luHz\n", x, y, frequency);
                goto done;
            }
            // skip the until the end of range for the blocking sensor
            unsigned y_range = sensors[blockingSensor].range - abs(sensors[blockingSensor].x - x);
            // last y in range
            y = sensors[blockingSensor].y + y_range;
        }
    }
    printf("Could not find distress beacon :(\n");
done:
    return 0;
}
