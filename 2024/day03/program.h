#include <stdbool.h>

int process(char *input, bool conditionals) {
    FILE *inputFile = fopen(input, "r");
    int sum = 0;

    if (inputFile) {
        char c = fgetc(inputFile);
        char parenthesis1;
        int number1;
        int number2;
        char parenthesis2;
        bool enabled = true;

        while (!feof(inputFile)) {
            if (enabled && c == 'm' && 
                fscanf(inputFile, "ul%c%d,%d%c", &parenthesis1, &number1, &number2, &parenthesis2) == 4 && 
                parenthesis1 == '(' && parenthesis2 == ')') {
                sum += number1 * number2;
            } else if (conditionals && c == 'd') {
                c = fgetc(inputFile);

                if (c == 'o') {
                    c = fgetc(inputFile);

                    if (c == '(') {
                        c = fgetc(inputFile);

                        enabled = c == ')';
                    } else if (c == 'n') {
                        c = fgetc(inputFile);

                        if (c == '\'') {
                            c = fgetc(inputFile);

                            if (c == 't') {
                                c = fgetc(inputFile);

                                if (c == '(') {
                                    c = fgetc(inputFile);

                                    if (c == ')') {
                                        enabled = false;
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                c = fgetc(inputFile);
            }
        }

        fclose(inputFile);
    }

    return sum;
}