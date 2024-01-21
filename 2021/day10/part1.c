/* Day 10, part 1 = 374061 */

#include "../../aoc.h"
#include "stack.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct CharStack *stack = (struct CharStack *)calloc(1, sizeof(struct CharStack));

        while (!feof(inputFile)) {
            char c = fgetc(inputFile);
            char opener;

            switch (c) {
                case '(':
                case '[':
                case '{':
                case '<':
                    push(stack, c);
                    break;
                case ')':
                case ']':
                case '}':
                case '>':
                    opener = pop(stack);

                    if (closer(opener) != c) {
                        switch (c) {
                            case ')':
                                answer += 3;
                                break;
                            case ']':
                                answer += 57;
                                break;
                            case '}':
                                answer += 1197;
                                break;
                            case '>':
                                answer += 25137;
                                break;
                        }
                    }

                    break;
                case '\n':
                    clear(stack);
                    break;
            }
        }

        fclose(inputFile);

        free(stack->data);
        free(stack);
    }

    print(aoc, INT, &answer);

    return 0;
}
