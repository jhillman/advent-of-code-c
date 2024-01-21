#include <stdio.h>

long simulate(char *input, int days) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        long timers[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        long timer;
        char ch;
        long count = 0;

        while (fscanf(inputFile, "%ld%c", &timer, &ch) >= 1) {
            ++timers[timer];
        }

        fclose(inputFile);
        
        for (int i = 0; i < days; i++) {
            long newFishCount = *timers;

            for (timer = 0; timer < 8; timer++) {
                timers[timer] = timers[timer + 1];
            }

            timers[6] += newFishCount;
            timers[8] = newFishCount;
        }

        for (int i = 0; i < 9; i++) {
            count += timers[i];
        }

        return count;
    }

    return 0;
}
