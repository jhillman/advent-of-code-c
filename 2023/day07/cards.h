#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum HandType {
    FIVE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FULL_HOUSE,
    THREE_OF_A_KIND,
    TWO_PAIR,
    ONE_PAIR,
    HIGH_CARD
};

struct Hand {
    char cards[6];
    enum HandType type;
    int bid;
};

struct Hands {
    int capacity;
    int count;
    struct Hand *data;
};

int compareInts(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

enum HandType getHandType(char cards[6], bool jokerRule) {
    char cardLabels[] = "JAKQT98765432";
    int cardLabelCounts[13] = { 0 };
    char *label = cards;
    int jokerCount = 0;

    while (*label) {
        ++cardLabelCounts[strchr(cardLabels, *label) - cardLabels];
        ++label;
    }

    if (jokerRule) {
        jokerCount = *cardLabelCounts;
        *cardLabelCounts = 0;
    }

    qsort(cardLabelCounts, 13, sizeof(int), compareInts);

    if (*cardLabelCounts == 5 || *cardLabelCounts + jokerCount == 5) {
        return FIVE_OF_A_KIND;
    } else if (*cardLabelCounts == 4 || *cardLabelCounts + jokerCount == 4) {
        return FOUR_OF_A_KIND;
    } else if (*cardLabelCounts == 3 || *cardLabelCounts + jokerCount == 3) {
        if (cardLabelCounts[1] == 2) {
            return FULL_HOUSE;
        } else {
            return THREE_OF_A_KIND;
        }
    } else if (*cardLabelCounts == 2 || *cardLabelCounts + jokerCount == 2) {
        if (cardLabelCounts[1] == 2) {
            return TWO_PAIR;
        } else {
            return ONE_PAIR;
        }
    } else {
        return HIGH_CARD;
    }
}

void addHand(struct Hands *hands, struct Hand hand) {
    if (hands->count == hands->capacity) {
        hands->capacity += 5;
        hands->data = (struct Hand *)realloc(hands->data, hands->capacity * sizeof(struct Hand));
    }

    hands->data[hands->count++] = hand;
}

int compareCards(char *cardsA, char *cardsB, bool jokerRule) {
    char cardLabels[14];
    char *cardA = cardsA;
    char *cardB = cardsB;

    strcpy(cardLabels, jokerRule ? "AKQT98765432J" : "AKQJT98765432");

    while (*cardA == *cardB) {
        ++cardA;
        ++cardB;
    }

    return (strchr(cardLabels, *cardB) - cardLabels) - (strchr(cardLabels, *cardA) - cardLabels);    
}

int compareHands(const void *a, const void *b) {
    int result = ((struct Hand *)b)->type - ((struct Hand *)a)->type;

    if (result == 0) {
        result = compareCards(((struct Hand *)a)->cards, ((struct Hand *)b)->cards, false);
    }

    return result;
}

int compareHandsJokerRule(const void *a, const void *b) {
    int result = ((struct Hand *)b)->type - ((struct Hand *)a)->type;

    if (result == 0) {
        result = compareCards(((struct Hand *)a)->cards, ((struct Hand *)b)->cards, true);
    }

    return result;
}

int getWinnings(char *input, bool jokerRule) {
    FILE *inputFile = fopen(input, "r");
    int winnings = 0;

    if (inputFile) {
        char line[16];
        struct Hands *hands = (struct Hands *)calloc(1, sizeof(struct Hands));
        struct Hand hand;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            sscanf(line, "%s %d", hand.cards, &hand.bid);
            hand.type = getHandType(hand.cards, jokerRule);

            addHand(hands, hand);
        }

        fclose(inputFile);

        qsort(hands->data, hands->count, sizeof(struct Hand), jokerRule ? compareHandsJokerRule : compareHands);

        for (int i = 0; i < hands->count; i++) {
            winnings += (hands->data[i].bid * (i + 1));
        }

        free(hands->data);
        free(hands);
    }

    return winnings;
}
