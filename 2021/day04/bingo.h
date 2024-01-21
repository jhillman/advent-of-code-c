#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 5
#define MARK -1

struct Location {
    bool exists;
    int x;
    int y;
};

struct Board {
    int squares[BOARD_SIZE][BOARD_SIZE];
    int unmarkedSum;
    struct Location numberLocations[100];
    bool winner;
};

struct BingoData {
    int *numbers;
    int numberCount;

    struct Board *boards;
    int boardCount;
};

void playNumber(struct Board *board, int number) {
    struct Location location = board->numberLocations[number];

    if (location.exists) {
        board->squares[location.y][location.x] = MARK;
        board->unmarkedSum -= number;
    }
}

bool boardWins(struct Board *board) {
    bool win = false;

    for (int y = 0; !win && y < BOARD_SIZE; y++) {
        win = true;

        for (int x = 0; win && x < BOARD_SIZE; x++) {
            win = board->squares[y][x] == MARK;
        }
    }

    for (int x = 0; !win && x < BOARD_SIZE; x++) {
        win = true;

        for (int y = 0; win && y < BOARD_SIZE; y++) {
            win = board->squares[y][x] == MARK;
        }
    }

    board->winner = win;

    return win;
}

void printBoard(struct Board board) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            printf("%2d ", board.squares[y][x]);
        }

        printf("\n");
    }    

    printf("\n");
}

struct BingoData *getBingoData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct BingoData *data = (struct BingoData *)calloc(1, sizeof(struct BingoData));

        int number;
        char ch;

        while (fscanf(inputFile, "%d%c", &number, &ch) >= 1) {
            ++data->numberCount;

            if (ch != ',') {
                int spotCount = 0;

                while (fscanf(inputFile, "%d%c", &number, &ch) >= 1) {
                    if (++spotCount == BOARD_SIZE * BOARD_SIZE) {
                        ++data->boardCount;
                        spotCount = 0;
                    }
                }
            }
        }

        data->numbers = (int *)calloc(data->numberCount, sizeof(int));

        fseek(inputFile, 0, SEEK_SET);

        for (int i = 0; i < data->numberCount; i++) {
            fscanf(inputFile, "%d%c", &number, &ch);

            data->numbers[i] = number;
        }

        data->boards = (struct Board *)calloc(data->boardCount, sizeof(struct Board));

        for (int i = 0; i < data->boardCount; i++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                for (int x = 0; x < BOARD_SIZE; x++) {
                    fscanf(inputFile, "%d%c", &number, &ch);

                    data->boards[i].squares[y][x] = number;
                    data->boards[i].unmarkedSum += number;
                    data->boards[i].numberLocations[number].exists = true;
                    data->boards[i].numberLocations[number].x = x;
                    data->boards[i].numberLocations[number].y = y;
                }
            }
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeBingoData(struct BingoData *data) {
    free(data->numbers);
    free(data->boards);
    free(data);
}
