#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

void handleMove(int moveAmount, char **map, int width, int height, int cubeWidth, char *facing, int *x, int *y);

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

enum Instruction {
    MOVE,
    ROTATE_CLOCKWISE,
    ROTATE_COUNTER_CLOCKWISE,
    STOP
};

enum Instruction readInstruction(char **instruction, int *moveAmount) {
    if (isdigit(**instruction)) {
        sscanf(*instruction, "%d", moveAmount);

        *instruction += (int)(*moveAmount == 0 ? 1 : log10(*moveAmount) + 1);

        return MOVE;
    } else if (**instruction == 'R') {
        ++*instruction;

        return ROTATE_CLOCKWISE;
    } else if (**instruction == 'L') {
        ++*instruction;

        return ROTATE_COUNTER_CLOCKWISE;
    } else {
        return STOP;
    }
}

int facingValue(char facing) {
    switch (facing) {
    case '^':
        return 3;
    case '<':
        return 2;
    case '>':
        return 0;
    case 'v':
        return 1;
    }

    return 0;
}

int navigateMap(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[256];
        int width = 0;
        int height = 0;
        int cubeWidth = 1000;
        char **map;
        char instructionsString[8192];
        int x = 0;
        int y = 0;
        char facing = '>';

        while (fgets(line, sizeof(line), inputFile) && *line == ' ' || *line == '.' || *line == '#') {
            line[strcspn(line, "\n")] = 0;
            width = max(width, strlen(line));
            cubeWidth = min(cubeWidth, strlen(line));

            if (height == 0) {
                x = strchr(line, '.') - line;
            }

            ++height;
        }

        map = (char **)malloc(height * sizeof(char *));

        fseek(inputFile, 0, SEEK_SET);

        for (int y = 0; y < height; y++) {
            map[y] = (char *)calloc((width + 1), sizeof(char));
            memset(map[y], ' ', width);

            fgets(line, sizeof(line), inputFile);
            line[strcspn(line, "\n")] = 0;

            strcpy(map[y], line);     
            
            if (strlen(line) < width - 1) {
                map[y][strlen(line)] = ' ';       
            }
        }

        fgets(line, sizeof(line), inputFile);
        fgets(instructionsString, sizeof(instructionsString), inputFile);

        fclose(inputFile);

        char *instructionPtr = instructionsString;
        int moveAmount;

        enum Instruction instruction = readInstruction(&instructionPtr, &moveAmount);

        while (instruction != STOP) {
            switch (instruction) {
            case MOVE:
                handleMove(moveAmount, map, width, height, cubeWidth, &facing, &x, &y);
                break;
            case ROTATE_CLOCKWISE:
                switch (facing) {
                case '^':
                    facing = '>';
                    break;
                case '<':
                    facing = '^';
                    break;
                case '>':
                    facing = 'v';
                    break;
                case 'v':
                    facing = '<';
                    break;
                }
                map[y][x] = facing;
                break;
            case ROTATE_COUNTER_CLOCKWISE:
                switch (facing) {
                case '^':
                    facing = '<';
                    break;
                case '<':
                    facing = 'v';
                    break;
                case '>':
                    facing = '^';
                    break;
                case 'v':
                    facing = '>';
                    break;
                }
                map[y][x] = facing;
                break;
            case STOP:
                break;
            }

            instruction = readInstruction(&instructionPtr, &moveAmount);
        }

        for (int y = 0; y < height; y++) {
            free(map[y]);
        }

        free(map);

        return (1000 * (y + 1)) + (4 * (x + 1)) + facingValue(facing);
    }

    return 0;
}
