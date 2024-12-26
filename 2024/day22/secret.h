#include <stdlib.h>
#include <stdbool.h>

struct Secrets {
    int capacity;
    int count;
    long *data;
};

void addSecret(struct Secrets *secrets, long value) {
    if (secrets->count == secrets->capacity) {
        secrets->capacity += 100;
        secrets->data = (long *)realloc(secrets->data, secrets->capacity * sizeof(long));
    }

    secrets->data[secrets->count++] = value;
}

void freeSecrets(struct Secrets *secrets) {
    free(secrets->data);
    free(secrets);
}

struct Secrets *getSecrets(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Secrets *secrets = NULL;

    if (inputFile) {
        long value;
        secrets = (struct Secrets *)calloc(1, sizeof(struct Secrets));

        while (!feof(inputFile) && fscanf(inputFile, "%ld", &value)) {
            addSecret(secrets, value);
        }

        fclose(inputFile);
    }

    return secrets;    
}

struct ChangeSequence {
    int data[4];
    int *change;
    int *first;
    int change1;
    int change2;
    int change3;
    int change4;
    bool ready;
};

int advance(struct ChangeSequence *sequence, int **change) {
    int value = **change;

    ++*change;

    if (*change - sequence->data == 4) {
        *change = sequence->data;
        sequence->ready = true;
    }

    return value;
}

void update(struct ChangeSequence *sequence) {
    int *change = sequence->first;

    sequence->change1 = advance(sequence, &change) + 9;
    sequence->change2 = advance(sequence, &change) + 9;
    sequence->change3 = advance(sequence, &change) + 9;
    sequence->change4 = advance(sequence, &change) + 9;

    advance(sequence, &sequence->first);
}

struct Secret {
    long value;
    int price;
    int previous;
    struct ChangeSequence *sequence;
};

void initialize(struct Secret *secret, long value) {
    secret->value = value;
    secret->previous = value % 10L;
    secret->price = 0;

    if (secret->sequence) {
        secret->sequence->change = secret->sequence->data;
        secret->sequence->first = secret->sequence->data;
        secret->sequence->ready = false;
    }
}

void next(struct Secret *secret) {
    secret->value = (secret->value ^ secret->value << 6)  & 16777215L;
    secret->value = (secret->value ^ secret->value >> 5)  & 16777215L;
    secret->value = (secret->value ^ secret->value << 11) & 16777215L;

    if (secret->sequence) {
        secret->price = secret->value % 10L;

        *secret->sequence->change = secret->price - secret->previous;

        advance(secret->sequence, &secret->sequence->change);
    }

    secret->previous = secret->price;
}

struct SequencePrice {
    bool set;
    int total;
};

struct SequencePrice ****getSequencePrices() {
    struct SequencePrice ****sequencePrices = (struct SequencePrice ****)malloc(20 * sizeof(struct SequencePrice ***));

    for (int x1 = 0; x1 < 20; x1++) {
        sequencePrices[x1] = (struct SequencePrice ***)malloc(20 * sizeof(struct SequencePrice **));
        
        for (int x2 = 0; x2 < 20; x2++) {
            sequencePrices[x1][x2] = (struct SequencePrice **)malloc(20 * sizeof(struct SequencePrice *));

            for (int x3 = 0; x3 < 20; x3++) {
                sequencePrices[x1][x2][x3] = (struct SequencePrice *)malloc(20 * sizeof(struct SequencePrice));
            }
        }
    }

    return sequencePrices;
}

void reset(struct SequencePrice ****sequencePrices) {
    for (int x1 = 0; x1 < 20; x1++) {
        for (int x2 = 0; x2 < 20; x2++) {
            for (int x3 = 0; x3 < 20; x3++) {
                for (int x4 = 0; x4 < 20; x4++) {
                    sequencePrices[x1][x2][x3][x4].set = false;
                }
            }
        }
    }
}

void freeSequencePrices(struct SequencePrice ****sequencePrices) {
    for (int x1 = 0; x1 < 20; x1++) {
        for (int x2 = 0; x2 < 20; x2++) {
            for (int x3 = 0; x3 < 20; x3++) {
                free(sequencePrices[x1][x2][x3]);
            }

            free(sequencePrices[x1][x2]);
        }

        free(sequencePrices[x1]);
    }

    free(sequencePrices);
}
