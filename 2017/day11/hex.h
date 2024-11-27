#include <stdlib.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

enum Type {
    FEWEST,
    MAX
};

int steps(char *input, enum Type type) {
    FILE *inputFile = fopen(input, "r");
    int result = 0;

    if (inputFile) {
        char direction[3];
        char first;
        char second;
        int x = 0;
        int y = 0;
        int z = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%2[^,\n]%*[,\n]", direction)) {
            first = *direction;
            second = *(direction + 1);

            switch (first) {
            case 'n':
                ++z;

                switch (second) {
                case 'e':
                    ++x;
                    break;
                case 'w':
                    --x;
                    break;
                default:
                    ++y;
                    break;
                }

                break;
            case 's':
                --z;

                switch (second) {
                case 'e':
                    ++x;
                    break;
                case 'w':
                    --x;
                    break;
                default:
                    --y;
                    break;
                }
                
                break;
            }

            if (type == MAX) {
                result = max(result, (abs(x) + abs(y) + abs(z)) / 2);
            }
        }

        fclose(inputFile);

        if (type == FEWEST) {
            result = (abs(x) + abs(y) + abs(z)) / 2;
        }
    }

    return result;
}