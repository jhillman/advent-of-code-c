#include <stdio.h>
#include <string.h>

void handleCycle(int cycle, int x, void *output);

void runCpu(char *input, void *output) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char instruction[8];
        int value;
        char nl;
        int cycle = 1;
        int x = 1;

        while (fscanf(inputFile, "%s %d%c", instruction, &value, &nl) >= 1) {
            if (strcmp(instruction, "noop") == 0) {
                handleCycle(cycle++, x, output);
            } else if (strcmp(instruction, "addx") == 0) {
                handleCycle(cycle++, x, output);
                handleCycle(cycle++, x, output);
                
                x += value;
            }
        }   

        handleCycle(cycle, x, output);
             
        fclose(inputFile);
    }
}