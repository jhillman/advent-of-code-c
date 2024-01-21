/* Day 10, part 2 = 2116639949 */

#include "../../aoc.h"
#include "stack.h"

int compare(const void *a, const void *b) {
    return *(long *)a - *(long *)b > 0 ? 1 : -1;
}

long incompleteLineScore(struct CharStack *stack) {
    long score = 0;
    long value;

    while (stack->size) {
        switch (pop(stack)) {
            case '(':
                value = 1;
                break;
            case '[':
                value = 2;
                break;
            case '{':
                value = 3;
                break;
            case '<':
                value = 4;
                break;
        }

        score = (score * 5) + value;
    }

    return score;
}

long *addScore(long *scores, int *scoreCount, long score) {
    scores = (long *)realloc(scores, (*scoreCount + 1) * sizeof(long));

    scores[*scoreCount] = score;

    *scoreCount = *scoreCount + 1;

    return scores;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct CharStack *stack = (struct CharStack *)calloc(1, sizeof(struct CharStack));
        long *scores = (long *)malloc(sizeof(long));
        int scoreCount = 0;

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
                        while (!feof(inputFile) && c != '\n') {
                            c = fgetc(inputFile);
                        }

                        clear(stack);
                    }

                    break;
                case '\n':
                    scores = addScore(scores, &scoreCount, incompleteLineScore(stack));
                    break;
            }
        }

        fclose(inputFile);

        if (stack->size > 0) {
            scores = addScore(scores, &scoreCount, incompleteLineScore(stack));
        }

        qsort(scores, scoreCount, sizeof(long), compare);

        answer = scores[scoreCount / 2];

        free(stack->data);
        free(stack);
        free(scores);
    }

    print(aoc, INT, &answer);

    return 0;
}
