#include <stdlib.h>
#include <stdbool.h>

int mazeSteps(char *input, void (*updateOffset)(int *, int)) {
    FILE *inputFile = fopen(input, "r");
    int steps = 0;

    if (inputFile) {
        int length = 0;
        int *maze;
        int value;
        int instruction = 0;
        int jump;

        while (!feof(inputFile)) {
            if (fgetc(inputFile) == '\n' || feof(inputFile)) {
                ++length;
            }
        }

        maze = (int *)malloc(length * sizeof(int));

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile) && fscanf(inputFile, "%d", &value)) {
            maze[instruction++] = value;
        }

        fclose(inputFile);

        instruction = 0;
        jump = maze[instruction];

        while (true) {
            updateOffset(maze, instruction);
            instruction += jump;
            ++steps;

            jump = maze[instruction];

            if (instruction + jump < 0 || instruction + jump >= length) {
                ++steps;
                break;
            }
        }

        free(maze);
    }

    return steps;
}
