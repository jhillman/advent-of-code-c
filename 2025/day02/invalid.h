#include <math.h>
#include <stdbool.h>

int digitCount(long number) {
    return (int)floor(log10((double)number)) + 1;
}

int firstSequenceLength(int digits);

long invalidSum(char *input) {
    FILE *inputFile = fopen(input, "r");
    long invalidSum = 0;

    if (inputFile) {
        long first;
        long last;
        char character;

        while (fscanf(inputFile, "%ld-%ld%c", &first, &last, &character) >= 2) {
            long id = first;

            while (id <= last) {
                int digits = digitCount(id);
                int maxSequenceLength = digits / 2;
                int sequenceLength = firstSequenceLength(digits);
                bool sequenceRepeated = false;

                while (!sequenceRepeated && sequenceLength <= maxSequenceLength) {
                    long modulo = pow(10, sequenceLength);

                    long segment = id % modulo;
                    long remaining = id / modulo;
                    long nextSegment = remaining % modulo;

                    if (segment == nextSegment) {
                        sequenceRepeated = true;

                        while (sequenceRepeated && remaining != remaining % modulo) {
                            remaining /= modulo;
                            nextSegment = remaining % modulo;

                            sequenceRepeated = segment == nextSegment;
                        } 
                    }

                    if (sequenceRepeated) {
                        break;
                    }

                    do {
                        ++sequenceLength;
                    } while (digits % sequenceLength != 0);
                }

                if (sequenceRepeated) {
                    invalidSum += id;
                }

                ++id;
            }
        }

        fclose(inputFile);
    }

    return invalidSum;

    return 0;
}
