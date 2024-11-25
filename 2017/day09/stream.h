#include <stdbool.h>

enum Type {
    SCORE,
    GARBAGE
};

int process(char *input, enum Type type) {
    FILE *inputFile = fopen(input, "r");
    int result = 0;

    if (inputFile) {
        char ch;
        int score = 0;
        int total = 0;
        int garbage = 0;

        while (!feof(inputFile) && (ch = fgetc(inputFile))) {
            switch (ch) {
            case '!':
                fgetc(inputFile);
                break;
            case '<':
                ch = fgetc(inputFile);

                while (ch != '>') {
                    if (ch == '!') {
                        fgetc(inputFile);
                    } else {
                        ++garbage;
                    }

                    ch = fgetc(inputFile);
                }
                break;
            case '{':
                ++score;
                break;
            case '}':
                total += score--;
                break;
            }
        }

        fclose(inputFile);

        switch (type) {
        case SCORE:
            result = total;
            break;
        case GARBAGE:
            result = garbage;
            break;
        }
    }

    return result;
}
