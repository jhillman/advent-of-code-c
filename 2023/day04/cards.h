#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Card {
    int winners;
    int count;
};

struct Cards {
    int count;
    struct Card *cards;
};

struct Cards *getCards(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Cards *cards = NULL;

    if (inputFile) {
        char c;
        char line[128];
        char *position;
        char numbers[100];
        int number;
        bool checkingNumbers;
        int cardIndex;

        cards = (struct Cards *)calloc(1, sizeof(struct Cards *));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++cards->count;
            }
        }

        cards->cards = (struct Card *)calloc(cards->count, sizeof(struct Card));

        for (int i = 0; i < cards->count; i++) {
            cards->cards[i].count = 1;
        }

        fseek(inputFile, 0, SEEK_SET);

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            position = line + 5;

            sscanf(position, "%d", &cardIndex);

            --cardIndex;

            position = strchr(position, ':') + 2;
            checkingNumbers = false;
            memset(numbers, 0, 100);

            while (*position && sscanf(position, "%d", &number)) {
                if (checkingNumbers && numbers[number]) {
                    ++cards->cards[cardIndex].winners;
                } else {
                    numbers[number] = 1;
                }

                position += 3;

                if (*position == '|') {
                    checkingNumbers = true;
                    position += 2;
                }
            }
        }
    }

    return cards;
}

void freeCards(struct Cards *cards) {
    free(cards->cards);
    free(cards);
}