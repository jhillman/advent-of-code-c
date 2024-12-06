#include <stdlib.h>
#include <stdbool.h>

struct Pages {
    int capacity;
    int count;
    int *data;
};

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

void addPage(struct Pages *pages, int page) {
    if (pages->count == pages->capacity) {
        pages->capacity += 10;
        pages->data = (int *)realloc(pages->data, pages->capacity * sizeof(int));
    }

    pages->data[pages->count++] = page;

    qsort(pages->data, pages->count, sizeof(int), compare);
}

int findPage(struct Pages *pages, int page, int low, int high) {
    if (pages->count == 0 || low > high) {
        return 0;
    }

    int middle = low + ((high - low) / 2);

    if (middle < pages->count) {
        int result = page - pages->data[middle];

        if (result == 0) {
            return pages->data[middle];
        } else if (result < 0) {
            return findPage(pages, page, low, middle - 1);
        } else {
            return findPage(pages, page, middle + 1, high);
        }
    }

    return 0;
}

bool updateValid(struct Pages *pages, int *update, int length) {
    bool valid = true;

    for (int i = 0; valid && i < length; i++) {
        for (int j = 0; valid && j < i; j++) {
            valid = !findPage(&pages[update[i]], update[j], 0, pages[update[i]].count);
        }
    }

    return valid;
}

void processUpdate(struct Pages *pages, int *update, int length, bool valid, int *sum);

void processUpdates(char *input, int *sum) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Pages pages[100] = { 0 };
        char line[128];
        int pageX;
        int pageY;
        int update[32];
        int length;
        char *page;

        while (!feof(inputFile) && fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            if (strlen(line) == 0) {
                break;
            }

            sscanf(line, "%d|%d", &pageX, &pageY);

            addPage(&pages[pageX], pageY);
        }

        while (!feof(inputFile) && fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            length = 0;
            page = strtok(line, ",");

            while (page) {
                update[length++] = atoi(page);

                page = strtok(NULL, ",");
            }

            processUpdate(pages, update, length, updateValid(pages, update, length), sum);
        }

        fclose(inputFile);

        for (int i = 0; i < 100; i++) {
            if (pages[i].data) {
                free(pages[i].data);
            }
        }
    }    
}