/* Day 9, part 2 = 6239783302560 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

struct File {
    int id;
    int offset;
    int size;
};

struct Disk {
    int capacity;
    int size;
    struct File *files;
};

void addFile(struct Disk *disk, struct File file) {
    if (disk->size == disk->capacity) {
        disk->capacity += 10000;
        disk->files = (struct File *)realloc(disk->files, disk->capacity * sizeof(struct File));
    }

    disk->files[disk->size++] = file;
}

struct Gaps {
    int capacity;
    int count;
    int *data;
};

int compare(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

void addGap(struct Gaps *gaps, int offset, bool sort) {
    if (gaps->count == gaps->capacity) {
        gaps->capacity += 1000;
        gaps->data = (int *)realloc(gaps->data, gaps->capacity * sizeof(int));
    }

    gaps->data[gaps->count++] = offset;

    if (sort) {
        qsort(gaps->data, gaps->count, sizeof(int), compare);
    }
}

int peekOffset(struct Gaps *gaps) {
    return gaps->data[gaps->count - 1];
}

int popOffset(struct Gaps *gaps) {
    return gaps->data[--gaps->count];
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        struct Disk *disk = (struct Disk *)calloc(1, sizeof(struct Disk));
        struct Gaps *gaps[10];
        int index = 0;
        int id = 0;
        int offset = 0;
        int size;
        long checksum = 0;

        for (int i = 0; i < 10; i++) {
            gaps[i] = (struct Gaps *)calloc(1, sizeof(struct Gaps));
        }

        while (!feof(inputFile)) {
            size = fgetc(inputFile) - '0';

            if (index++ % 2 == 0) {
                addFile(disk, (struct File){ id++, offset, size });
            } else if (size > 0) {
                addGap(gaps[size], offset, false);
            }

            offset += size;
        }

        fclose(inputFile);

        for (int i = 0; i < 10; i++) {
            qsort(gaps[i]->data, gaps[i]->count, sizeof(int), compare);
        }

        for (int i = disk->size - 1; i >= 0; i--) {
            struct File file = disk->files[i];
            offset = file.offset;
            size = 10;

            for (int s = file.size; s < 10; s++) {
                if (gaps[s]->count > 0 && peekOffset(gaps[s]) <= offset) {
                    offset = peekOffset(gaps[s]);
                    size = s;
                }
            }

            if (offset >= file.offset) {
                continue;
            }

            offset = popOffset(gaps[size]);
            disk->files[i].offset = offset;

            if (size > file.size) {
                addGap(gaps[size - file.size], offset + file.size, true);
            }
        }

        for (int i = 0; i < disk->size; i++) {
            for (long offset = disk->files[i].offset; offset < disk->files[i].offset + disk->files[i].size; offset++) {
                checksum += (long)disk->files[i].id * (long)offset;
            }
        }

        free(disk->files);
        free(disk);

        for (int i = 0; i < 10; i++) {
            free(gaps[i]->data);
            free(gaps[i]);
        }

        answer = checksum;
    }

    print(aoc, LONG, &answer);

    return 0;
}
