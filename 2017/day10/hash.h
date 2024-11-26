#include <stdlib.h>

#define LIST_LENGTH 256

struct Numbers {
    int list[LIST_LENGTH];
    int position;
    int skip;
};

void knot(struct Numbers *numbers, int length) {
    int start = numbers->position;
    int end = numbers->position + length - 1;
    int swaps = (length % LIST_LENGTH) / 2;

    for (int i = 0; i < swaps; i++) {
        numbers->list[start % LIST_LENGTH] += numbers->list[end % LIST_LENGTH];
        numbers->list[end % LIST_LENGTH] = numbers->list[start % LIST_LENGTH] - numbers->list[end % LIST_LENGTH];
        numbers->list[start % LIST_LENGTH] -= numbers->list[end % LIST_LENGTH];

        ++start;
        --end;
    }

    numbers->position = (numbers->position + length + numbers->skip++) % LIST_LENGTH;
}

struct Numbers *getNumbers() {
    struct Numbers *numbers = (struct Numbers *)calloc(1, sizeof(struct Numbers));
    
    for (int i = 0; i < LIST_LENGTH; i++) {
        numbers->list[i] = i;
    }

    return numbers;
}
