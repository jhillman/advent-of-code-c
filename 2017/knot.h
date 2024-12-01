#include <stdlib.h>

struct Numbers {
    int *list;
    int length;
    int position;
    int skip;
};

struct Numbers *getNumbers(int length) {
    struct Numbers *numbers = (struct Numbers *)calloc(1, sizeof(struct Numbers));
    numbers->length = length;
    numbers->list = (int *)calloc(length, sizeof(int));

    for (int i = 0; i < length; i++) {
        numbers->list[i] = i;
    }

    return numbers;
}

void knot(struct Numbers *numbers, int length) {
    int start = numbers->position;
    int end = numbers->position + length - 1;
    int swaps = (length % numbers->length) / 2;

    for (int i = 0; i < swaps; i++) {
        numbers->list[start % numbers->length] += numbers->list[end % numbers->length];
        numbers->list[end % numbers->length] = numbers->list[start % numbers->length] - numbers->list[end % numbers->length];
        numbers->list[start % numbers->length] -= numbers->list[end % numbers->length];

        ++start;
        --end;
    }

    numbers->position = (numbers->position + length + numbers->skip++) % numbers->length;
}

char *hash(char *input, char *output) {
    struct Numbers *numbers = getNumbers(256);
    char *c;
    int hash;

    for (int i = 0; i < 64; i++) {
        c = input;

        while (*c) {
            knot(numbers, (int)*c++);
        }

        knot(numbers, 17);
        knot(numbers, 31);
        knot(numbers, 73);
        knot(numbers, 47);
        knot(numbers, 23);
    }

    c = output;

    for (int i = 0; i < 16; i++) {
        hash = 0;

        for (int j = 0; j < 16; j++) {
            hash ^= numbers->list[i * 16 + j];
        }

        sprintf(c, "%02x", hash);
        c += 2;
    }

    free(numbers->list);
    free(numbers);

    return output;
}
