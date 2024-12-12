/* Day 9, part 1 = 6211348208140 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

struct Values {
    int capacity;
    int count;
    int *data;
};

void addValue(struct Values *values, int offset) {
    if (values->count == values->capacity) {
        values->capacity += 10000;
        values->data = (int *)realloc(values->data, values->capacity * sizeof(int));
    }

    values->data[values->count++] = offset;
}

void freeValues(struct Values *values) {
    free(values->data);
    free(values);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        struct Values *files = (struct Values *)calloc(1, sizeof(struct Values));
        struct Values *gaps = (struct Values *)calloc(1, sizeof(struct Values));
        struct Values *disk = (struct Values *)calloc(1, sizeof(struct Values));
        int index = 0;

        while (!feof(inputFile)) {
            addValue(index++ % 2 == 0 ? files : gaps, fgetc(inputFile) - '0');
        }

        fclose(inputFile);

        int *start = files->data;
        int *end = files->data + (files->count - 1);
        int *gap = gaps->data;
        long checksum = 0;

        while (start < end) {
            while (*start) {
                addValue(disk, start - files->data);
                --*start;
            }

            if (!*gap) {
                gap++;
            }

            while (*gap && end > start) {
                while (*gap && *end) {
                    addValue(disk, end - files->data);
                    --*gap;
                    --*end;
                }

                if (!*end && *gap) {
                    end--;
                }
            }

            start++;
        }

        for (int id = 0; id < disk->count; id++) {
            checksum += (id * disk->data[id]);
        }

        answer = checksum;

        freeValues(files);
        freeValues(gaps);
        freeValues(disk);
    }

    print(aoc, LONG, &answer);

    return 0;
}
