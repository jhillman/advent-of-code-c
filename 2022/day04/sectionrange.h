#include <stdio.h>
#include <stdbool.h>

struct SectionRange {
    int min;
    int max;
};

bool testSectionRanges(struct SectionRange range1, struct SectionRange range2);

int checkSectionAssignments(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        int result = 0;
        struct SectionRange sectionRange1;
        struct SectionRange sectionRange2;

        while (fscanf(inputFile, "%d-%d,%d-%d", &sectionRange1.min, &sectionRange1.max, &sectionRange2.min, &sectionRange2.max) > 0) {
            if (testSectionRanges(sectionRange1, sectionRange2)) {
                ++result;
            }
        }

        fclose(inputFile);

        return result;
    }

    return 0;
}