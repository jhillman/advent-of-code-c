#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

struct Schematic {
	int width;
	int height;
	char **grid;
};

struct Part {
	char part;
	int number;
	int x;
	int y;
};

int digitCount(int number) {
    return (int)floor(log10((double)number)) + 1;
}

struct Schematic *getSchematic(char *input) {
	FILE *inputFile = fopen(input, "r");
	struct Schematic *schematic = NULL;

    if (inputFile) {
        char ch;

        schematic = (struct Schematic *)calloc(1, sizeof(struct Schematic));

        ch = fgetc(inputFile);

        while (ch != '\n') {
            ++schematic->width;
            ch = fgetc(inputFile);
        }

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n' || feof(inputFile)) {
                ++schematic->height;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        schematic->grid = (char **)malloc(schematic->height * sizeof(char *));

        for (int y = 0; y < schematic->height; y++) {
            schematic->grid[y] = (char *)malloc(schematic->width * sizeof(char));

            for (int x = 0; x < schematic->width; x++) {
                schematic->grid[y][x] = fgetc(inputFile);
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return schematic;
}

void freeSchematic(struct Schematic *schematic) {
    for (int y = 0; y < schematic->height; y++) {
        free(schematic->grid[y]);
    }

    free(schematic->grid);
    free(schematic);
}

struct Part getPart(struct Schematic *schematic, int x, int y) {
	struct Part part = { '\0', -1, -1, -1 };

    if (isdigit(schematic->grid[y][x])) {
        int numberStart = x;
        int numberEnd = x;

        while (isdigit(schematic->grid[y][numberEnd]) && numberEnd < schematic->width) {
            ++numberEnd;
        }

        --numberEnd;
        bool partFound = false;

        for (int dy = y - 1; !partFound && dy <= y + 1; dy++) {
            for (int dx = numberStart - 1; !partFound && dx <= numberEnd + 1; dx++) {
                if (!(dx >= 0 && dx < schematic->width && dy >= 0 && dy < schematic->height)) {
                    continue;
                }

                if (dy == y && dx >= numberStart && dx <= numberEnd) {
                    continue;
                }

                if (!isdigit(schematic->grid[dy][dx]) && schematic->grid[dy][dx] != '.') {
                    char number[4];
                    strncpy(number, &schematic->grid[y][numberStart], numberEnd - numberStart + 1);
                    number[numberEnd - numberStart + 1] = '\0';

                    part.number = atoi(number);
                    part.part = schematic->grid[dy][dx];
                    part.x = dx;
                    part.y = dy;

                    partFound = true;
                }
            }
        }
    }

	return part;
}