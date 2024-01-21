#include <stdbool.h>

long evaluate(char *expression, int *expressionLength, bool advanced) {
    long value;
    long multiplicands[30];
    int multiplicandIndex = 0;
    char *operator = NULL;
    char *expressionStart = expression;
    int length;

    while (*expression) {
        switch (*expression) {
            case '(':
                if (operator) {
                    switch (*operator) {
                        case '+':
                            value += evaluate(expression + 1, &length, advanced);
                            break;
                        case '*':
                            if (advanced) {
                                multiplicands[multiplicandIndex++] = value;
                                value = evaluate(expression + 1, &length, advanced);
                            } else {
                                value *= evaluate(expression + 1, &length, advanced);
                            }
                            break;
                    }

                    operator = NULL;
                    expression += length;
                } else {
                    value = evaluate(expression + 1, &length, advanced);

                    expression += length;
                }
                break;
            case ')':
                for (int i = 0; i < multiplicandIndex; i++) {
                    value *= multiplicands[i];
                }

                if (expressionLength) {
                    *expressionLength = expression - expressionStart + 1;
                }

                return value;
            case '+':
            case '*':
                operator = expression;
                break;
            default:
                if (operator) {
                    switch (*operator) {
                        case '+':
                            value += (*expression - '0');
                            break;
                        case '*':
                            if (advanced) {
                                multiplicands[multiplicandIndex++] = value;
                                value = (*expression - '0');
                            } else {
                                value *= (*expression - '0');
                            }
                            break;
                    }

                    operator = NULL;
                } else {
                    value = *expression - '0';
                }
                break;
        }

        ++expression;

        while (*expression == ' ' || *expression == '\n') {
            ++expression;
        }
    }

    for (int i = 0; i < multiplicandIndex; i++) {
        value *= multiplicands[i];
    }

    return value;
}