#include <stdbool.h>

int digitCount(long number) {
    int count = 0;

    while (number > 0) {
      number /= 10;
      count++;
    }

    return count;
}

long pow10(int number) {
    long result = 1;

    for (int i = 0; i < number; i++) {
        result *= 10;
    }

    return result;
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
            int digits = digitCount(id);
            bool digitCountChanges = digitCount(last) != digits;
            long nextPow10 = 0;

            while (id <= last) {
                if (digitCountChanges && id > nextPow10) {
                    digits = digitCount(id);
                    nextPow10 = pow10(digits);
                }

                int maxSequenceLength = digits / 2;
                int sequenceLength = firstSequenceLength(digits);
                bool sequenceRepeated = false;

                while (!sequenceRepeated && sequenceLength <= maxSequenceLength) {
                    long segmentSize = pow10(sequenceLength);
                    long segment = id % segmentSize;
                    long remaining = id / segmentSize;
                    long nextSegment = remaining % segmentSize;

                    if (segment == nextSegment) {
                        sequenceRepeated = true;

                        while (sequenceRepeated && remaining != remaining % segmentSize) {
                            remaining /= segmentSize;
                            nextSegment = remaining % segmentSize;

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
