/* Day 15, part 2 = 251353 */

#include "../../aoc.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

struct Lens {
    char label[8];
    int focalLength;

    struct Lens *next;
    struct Lens *previous;
};

struct Box {
    struct Lens *lenses;
};

void handleStep(struct Box *box, char label[8], char operation, int focalLength) {
    struct Lens *lens = box->lenses;
    struct Lens *lastLens = lens;

    while (lens && strcmp(lens->label, label) != 0) {
        lastLens = lens;
        lens = lens->next;
    }

    if (operation == '=') {
        if (lens) {
            lens->focalLength = focalLength;
        } else if (lastLens) {
            lastLens->next = (struct Lens *)calloc(1, sizeof(struct Lens));
            lastLens->next->previous = lastLens;
            strcpy(lastLens->next->label, label);
            lastLens->next->focalLength = focalLength;
        } else {
            box->lenses = (struct Lens *)calloc(1, sizeof(struct Lens));
            strcpy(box->lenses->label, label);
            box->lenses->focalLength = focalLength;
        }
    } else if (operation == '-') {
        if (lens) {
            if (box->lenses == lens) {
                box->lenses = lens->next;
            }

            if (lens->next) {
                lens->next->previous = lens->previous;
            }

            if (lens->previous) {
                lens->previous->next = lens->next;
            }

            free(lens);
        }
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    if (inputFile) {
        char c;
        int hash;
        char label[8];
        int labelIndex;
        char operation;
        int focalLength;
        struct Box boxes[256] = { 0 };
        struct Lens *lens;
        int answer = 0;

        do {
            hash = 0;
            labelIndex = 0;
            focalLength = 0;
            c = fgetc(inputFile);

            while (isalpha(c)) {
                label[labelIndex++] = c;

                hash += c;
                hash *= 17;
                hash %= 256;

                c = fgetc(inputFile);
            }

            label[labelIndex] = '\0';
            operation = c;

            if (operation == '=') {
                fscanf(inputFile, "%d,", &focalLength);
            } else {
                fgetc(inputFile);
            }

            handleStep(&boxes[hash], label, operation, focalLength);

            for (int i = 0; i < 256; i++) {
                if (boxes[i].lenses) {
                    struct Lens *lens = boxes[i].lenses;

                    while (lens) {
                        lens = lens->next;
                    }
                }
            }
        } while (!feof(inputFile));

        fclose(inputFile);

        for (int i = 0; i < 256; i++) {
            struct Lens *lens = boxes[i].lenses;
            struct Lens *next;
            int lensPosition = 1;
            int focusingPower = 0;

            while (lens) {
                answer += (i + 1) * lensPosition * lens->focalLength;

                next = lens->next;
                ++lensPosition;

                free(lens);
                lens = next;
            }
        }

        print(aoc, INT, &answer);
    }

    return 0;
}
