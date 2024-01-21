#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Stack {
    int crateCount;
    int crateCapacity;
    char *crates;
    char *top;
};

enum CrateMoverVersion {
    V9000,
    V9001
};

char *strrev(char *string) {
    if (string && *string) {
        char *start = string;
        char *end = string + strlen(string) - 1;
        
        while (start < end) {
            char ch = *start;
            *start++ = *end;
            *end-- = ch;
        }
    }

    return string;
}

void moveCrates(char *input, enum CrateMoverVersion moverVersion, char *topCrates) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[40];
        fgets(line, sizeof(line), inputFile);

        int stackCount = (int)strlen(line) / 4;
        struct Stack *stacks = (struct Stack *)calloc(stackCount, sizeof(struct Stack));

        for (int i = 0; i < stackCount; i++) {
            stacks[i].crateCapacity = 50;
            stacks[i].crates = (char *)calloc(stacks[i].crateCapacity, sizeof(char));
        }

        while (!isdigit(line[1])) {
            char *crate = line + 1;

            for (int i = 0; i < stackCount; i++) {
                if (stacks[i].crateCount == stacks[i].crateCapacity) {
                    stacks[i].crateCapacity += 50;
                    stacks[i].crates = (char *)realloc(stacks[i].crates, stacks[i].crateCapacity * sizeof(char));
                }

                if (isalpha(*crate)) {
                    stacks[i].crates[stacks[i].crateCount++] = *crate;
                }

                crate += 4;
            }

            fgets(line, sizeof(line), inputFile);            
        }

        for (int i = 0; i < stackCount; i++) {
            strrev(stacks[i].crates);
            stacks[i].top = stacks[i].crates + stacks[i].crateCount - 1;
        }

        // blank line
        fgets(line, sizeof(line), inputFile);            

        int crateCount;
        int fromStack;
        int toStack;
        char ch;
        char crates[50];

        while (fscanf(inputFile, "move %d from %d to %d%c", &crateCount, &fromStack, &toStack, &ch) >= 3) {
            --fromStack;
            --toStack;
            
            if (stacks[toStack].crateCount == stacks[toStack].crateCapacity) {
                stacks[toStack].crateCapacity += 50;
                stacks[toStack].crates = (char *)realloc(stacks[toStack].crates, stacks[toStack].crateCapacity * sizeof(char));                    
            }

            switch (moverVersion) {
            case V9000:
                for (int i = 0; i < crateCount; i++) {
                    *++stacks[toStack].top = *stacks[fromStack].top--;
                }
                break;
            case V9001:
                strncpy(crates, stacks[fromStack].top - (crateCount - 1), crateCount);
                crates[crateCount] = 0;

                stacks[fromStack].top -= crateCount;

                strcpy(stacks[toStack].top + 1, crates);
                stacks[toStack].top += crateCount;
                break;
            }

            stacks[fromStack].crateCount -= crateCount;
            stacks[toStack].crateCount += crateCount;
        }

        fclose(inputFile);

        char *top = topCrates;

        for (int i = 0; i < stackCount; i++) {
            *top++ = *stacks[i].top;
            *top = '\0';

            free(stacks[i].crates);
        }

        free(stacks);
    }
}