#include <stdlib.h>

#define min(a,b) ((a) < (b) ? (a) : (b))

struct XY {
    long long x;
    long long y;
};

struct Machine {
    struct XY a;
    struct XY b;
    struct XY prize;
};

long long totalCost(char *input, long long offset) {
    FILE *inputFile = fopen(input, "r");
    struct Machines *machines = NULL;
    long long cost = 0;

    if (inputFile) {
        struct Machine machine;

        while (!feof(inputFile) && fscanf(inputFile, "Button A: X+%lld, Y+%lld\nButton B: X+%lld, Y+%lld\nPrize: X=%lld, Y=%lld ",
            &machine.a.x, &machine.a.y, 
            &machine.b.x, &machine.b.y, 
            &machine.prize.x, &machine.prize.y)) {
            machine.prize.x += offset;
            machine.prize.y += offset;

            // https://en.wikipedia.org/wiki/Cramer%27s_rule
            long long determinant = machine.a.x * machine.b.y - machine.a.y * machine.b.x;
            long long aPresses = (machine.prize.x * machine.b.y - machine.prize.y * machine.b.x) / determinant;
            long long bPresses = (machine.prize.y * machine.a.x - machine.prize.x * machine.a.y) / determinant;

            if (machine.a.x * aPresses + machine.b.x * bPresses == machine.prize.x && 
                machine.a.y * aPresses + machine.b.y * bPresses == machine.prize.y) {
                cost += 3LL * aPresses + bPresses;
            }
        }

        fclose(inputFile);
    }

    return cost;
}
