#include <stdlib.h>
#include <stdbool.h>

enum ReportType {
    INDETERMINATE,
    INCREASING,
    DECREASING,
    UNSAFE
};

enum ReportType reportType(int *levels, int levelCount, bool dampener, int skip) {
    enum ReportType type = INDETERMINATE;
    int level;
    int lastLevel = -1;

    for (int i = 0; i < levelCount; i++) {
        if (i == skip) {
            continue;
        }

        level = levels[i];

        if (lastLevel > 0) {
            switch (type) {
            case INDETERMINATE:
                if (level > lastLevel) {
                    type = INCREASING;
                } else if (level < lastLevel) {
                    type = DECREASING;
                } else {
                    type = UNSAFE;
                }
                break;
            case INCREASING:
                type = level > lastLevel ? INCREASING : UNSAFE;
                break;
            case DECREASING:
                type = level < lastLevel ? DECREASING : UNSAFE;
                break;
            case UNSAFE:
                break;
            }

            if (type != UNSAFE && abs(level - lastLevel) > 3) {
                type = UNSAFE;
            }
        }

        lastLevel = level;
    }

    if (type == UNSAFE && dampener) {
        for (int i = 0; type == UNSAFE && i < levelCount; i++) {
            type = reportType(levels, levelCount, false, i);
        }
    }

    return type;
}

int safeReportCount(char *input, bool dampener) {
    FILE *inputFile = fopen(input, "r");
    int safeCount = 0;

    if (inputFile) {
        char report[32];
        int levels[8];
        char *level;
        int levelCount;

        while (!feof(inputFile) && fgets(report, sizeof(report), inputFile)) {
            level = strtok(report, " ");
            levelCount = 0;

            while (level) {
                levels[levelCount++] = atoi(level);
                level = strtok(NULL, " ");
            }

            if (reportType(levels, levelCount, dampener, -1) != UNSAFE) {
                ++safeCount;
            }
        }

        fclose(inputFile);
    }

    return safeCount;
}