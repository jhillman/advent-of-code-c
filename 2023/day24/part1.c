/* Day 24, part 1 = 20336 */

#include "../../aoc.h"
#include "hailstones.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Hailstones *hailstones = getHailstones(aoc.input);
    int answer = 0;

    if (hailstones) {
        struct Hailstone hailstone1;
        struct Hailstone hailstone2;
        double slope1;
        double intercept1;
        double slope2;
        double intercept2;
        double areaMin = hailstones->count < 10 ? 7 : 200000000000000;
        double areaMax = hailstones->count < 10 ? 27 : 400000000000000;

        for (int i = 0; i < hailstones->count; i++) {
            hailstone1 = hailstones->data[i];
            slope1 = (double)hailstone1.velocity.y / (double)hailstone1.velocity.x;
            intercept1 = (double)hailstone1.position.y - slope1 * (double)hailstone1.position.x;

            for (int j = i + 1; j < hailstones->count; j++) {
                hailstone2 = hailstones->data[j];
                slope2 = (double)hailstone2.velocity.y / (double)hailstone2.velocity.x;
                intercept2 = (double)hailstone2.position.y - slope2 * (double)hailstone2.position.x;

                double crossX = (intercept2 - intercept1) / (slope1 - slope2);
                double crossY = slope1 * crossX + intercept1;

                if (crossX >= areaMin && crossX <= areaMax && crossY >= areaMin && crossY <= areaMax) {
                    if ((crossX - (double)hailstone1.position.x) / (double)hailstone1.velocity.x >= 0 && 
                        (crossX - (double)hailstone2.position.x) / (double)hailstone2.velocity.x >= 0) {
                        answer += 1;
                    }
                }
            }
        }

        free(hailstones->data);
        free(hailstones);
    }

    print(aoc, INT, &answer);

    return 0;
}
