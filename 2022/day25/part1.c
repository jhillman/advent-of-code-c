/* Day 25, part 1 = 2-0-020-1==1021=--01 */

#include "../../aoc.h"
#include <string.h>
#include <math.h>

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

long fromSNAFU(char *snafu) {
    long number = 0;
    int position = strlen(snafu) - 1;
    char *digit = snafu;

    while (*digit) {
        long value = pow(5, position);

        switch (*digit) {
        case '-':
            number += -1 * value;
            break;
        case '=':
            number += -2 * value;
            break;
        default:
            number += (*digit - '0') * value;
            break;
        }

        ++digit;
        --position;
    }

    return number;
}

void toSNAFU(long number, char *snafu) {
    char *digit = snafu;

    while (number) {
        long remainder = number % 5;

        number /= 5;

        if (remainder > 2) {
            remainder -= 5;
            ++number;
        }

        switch (remainder) {
        case -2:
            *digit++ = '=';
            break;
        case -1:
            *digit++ = '-';
            break;
        case 0:
            *digit++ = '0';
            break;
        case 1:
            *digit++ = '1';
            break;
        case 2:
            *digit++ = '2';
            break;
        }
    }

    *digit = 0;

    strrev(snafu);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    char snafu[32];

    if (inputFile) {
        long sum = 0;

        while (fgets(snafu, sizeof(snafu), inputFile)) {
            snafu[strcspn(snafu, "\n")] = 0;

            sum += fromSNAFU(snafu);
        }

        fclose(inputFile);

        toSNAFU(sum, snafu);
    }

    print(aoc, STRING, snafu);

    return 0;
}
