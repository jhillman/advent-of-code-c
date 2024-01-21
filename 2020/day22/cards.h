#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROUNDS 1200

struct Card {
    char value;
    struct Card *next;
};

struct Deck {
    struct Card *top;
    struct Card *bottom;
    int count;
};

struct Round {
    struct Deck *playerOneDeck;
    struct Deck *playerTwoDeck;
};

struct GameData {
    struct Deck *playerOneDeck;
    struct Deck *playerTwoDeck;
};

void addNewCard(struct Deck *deck, char value) {
    struct Card *card = (struct Card *) malloc(sizeof(struct Card));
    card->value = value;
    card->next = NULL;

    if (deck->bottom) {
        deck->bottom->next = card;
        deck->bottom = card;
    } else {
        deck->top = deck->bottom = card;
    }

    ++deck->count;
}

void addCard(struct Deck *deck, struct Card *card) {
    if (deck->bottom) {
        deck->bottom->next = card;
        deck->bottom = card;
    } else {
        deck->top = deck->bottom = card;
    }

    ++deck->count;
}

struct Card *drawCard(struct Deck *deck) {
    struct Card *card = deck->top;

    deck->top = card->next;

    if (--deck->count == 0) {
        deck->top = deck->bottom = NULL;
    }

    card->next = NULL;

    return card;
}

struct Deck *copyDeck(struct Deck *deck, int depth) {
    struct Deck *deckCopy = (struct Deck *) malloc(sizeof(struct Deck));
    deckCopy->top = deckCopy->bottom = NULL;
    deckCopy->count = 0;
    struct Card *card = deck->top;

    for (int i = 0; i < depth; i++) {
        addNewCard(deckCopy, card->value);

        card = card->next;
    }

    return deckCopy;
}

void freeCard(struct Card *card) {
    if (card) {
        freeCard(card->next);

        free(card);
    }
}

void freeDeck(struct Deck *deck) {
    if (deck) {
        freeCard(deck->top);

        free(deck);
    }
}

int playGame(struct Deck *playerOneDeck, struct Deck *playerTwoDeck, bool recursive) {
    int rounds = 0;
    bool duplicateFound = false;

    while (rounds < MAX_ROUNDS && playerOneDeck->count && playerTwoDeck->count) {
        struct Card *playerOneCard = drawCard(playerOneDeck);
        struct Card * playerTwoCard = drawCard(playerTwoDeck);

        if (recursive && playerOneCard->value <= playerOneDeck->count && playerTwoCard->value <= playerTwoDeck->count) {
            struct Deck *playerOneDeckCopy = copyDeck(playerOneDeck, playerOneCard->value);
            struct Deck *playerTwoDeckCopy = copyDeck(playerTwoDeck, playerTwoCard->value);

            if (playGame(playerOneDeckCopy, playerTwoDeckCopy, true) == 1) {
                addCard(playerOneDeck, playerOneCard);
                addCard(playerOneDeck, playerTwoCard);
            } else {
                addCard(playerTwoDeck, playerTwoCard);
                addCard(playerTwoDeck, playerOneCard);
            }

            freeDeck(playerOneDeckCopy);
            freeDeck(playerTwoDeckCopy);
        } else if (playerOneCard->value > playerTwoCard->value) {
            addCard(playerOneDeck, playerOneCard);
            addCard(playerOneDeck, playerTwoCard);
        } else {
            addCard(playerTwoDeck, playerTwoCard);
            addCard(playerTwoDeck, playerOneCard);
        }

        ++rounds;
    }

    if (rounds >= MAX_ROUNDS || playerOneDeck->count > playerTwoDeck->count) {
        return 1;
    } else {
        return 2;
    }
}

int deckScore(struct Deck *deck) {
    int score = 0;

    while (deck->count) {
        score += deck->count * drawCard(deck)->value;
    }

    return score;
}

struct GameData *readGameData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char *line = (char *) malloc(11 * sizeof(char));;
        size_t length;
        int number;
        struct Deck *deck;

        struct GameData *data = (struct GameData *) calloc(1, sizeof(struct GameData));

        data->playerOneDeck = (struct Deck *) calloc(1, sizeof(struct Deck));
        data->playerTwoDeck = (struct Deck *) calloc(1, sizeof(struct Deck));

        while(getline(&line, &length, inputFile) != -1) {
            if (sscanf(line, "Player %d:", &number)) {
                if (number == 1) {
                    deck = data->playerOneDeck;
                } else {
                    deck = data->playerTwoDeck;
                }
            } else if (sscanf(line, "%d", &number) == 1) {
                addNewCard(deck, number);
            }
        }

        free(line);
        
        fclose(inputFile);

        return data;
    }

    return NULL;
}