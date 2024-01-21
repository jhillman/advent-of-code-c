/* Day 21, part 2 = 634769613696613 */

#include "../../aoc.h"
#include <stdbool.h>

struct GameWins {
    long playerOne;
    long playerTwo;
};

struct GameTurn {
    int playerOnePosition;
    int playerTwoPosition;
    int playerOneScore;
    int playerTwoScore;
    bool playerOneTurn;
};

struct GameWins play(struct GameTurn lastTurn, struct GameWins cachedWins[11][11][30][30][2]) {
    struct GameWins wins = { 0, 0 };

    struct GameWins cachedWin = cachedWins[lastTurn.playerOnePosition][lastTurn.playerTwoPosition][lastTurn.playerOneScore][lastTurn.playerTwoScore][lastTurn.playerOneTurn ? 0 : 1];

    if (cachedWin.playerOne > 0 && cachedWin.playerTwo > 0) {
        return cachedWin;
    }

    for (int roll1 = 1; roll1 <= 3; roll1++) {
        for (int roll2 = 1; roll2 <= 3; roll2++) {
            for (int roll3 = 1; roll3 <= 3; roll3++) {
                int diceTotal = roll1 + roll2 + roll3;

                struct GameTurn currentTurn = {
                    lastTurn.playerOnePosition,
                    lastTurn.playerTwoPosition,
                    lastTurn.playerOneScore,
                    lastTurn.playerTwoScore,
                    lastTurn.playerOneTurn
                };

                if (currentTurn.playerOneTurn) {
                    if ((currentTurn.playerOnePosition = (currentTurn.playerOnePosition + diceTotal) % 10) == 0) {
                        currentTurn.playerOnePosition = 10;
                    }

                    currentTurn.playerOneScore += currentTurn.playerOnePosition;
                } else {
                    if ((currentTurn.playerTwoPosition = (currentTurn.playerTwoPosition + diceTotal) % 10) == 0) {
                        currentTurn.playerTwoPosition = 10;
                    }

                    currentTurn.playerTwoScore += currentTurn.playerTwoPosition;
                }

                if (currentTurn.playerOneScore >= 21) {
                    ++wins.playerOne;
                } else if (currentTurn.playerTwoScore >= 21) {
                    ++wins.playerTwo;
                } else {
                    currentTurn.playerOneTurn = !currentTurn.playerOneTurn;

                    struct GameWins nextTurnWins = play(currentTurn, cachedWins);

                    wins.playerOne += nextTurnWins.playerOne;
                    wins.playerTwo += nextTurnWins.playerTwo;
                }
            }
        }            
    }

    cachedWins[lastTurn.playerOnePosition][lastTurn.playerTwoPosition][lastTurn.playerOneScore][lastTurn.playerTwoScore][lastTurn.playerOneTurn ? 0 : 1] = wins;

    return wins;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        int playerOnePosition;
        int playerTwoPosition;
        struct GameWins cachedWins[11][11][30][30][2] = { { 0, 0 } };

        fscanf(inputFile, "Player 1 starting position: %d\n", &playerOnePosition);
        fscanf(inputFile, "Player 2 starting position: %d", &playerTwoPosition);

        fclose(inputFile);

        struct GameWins wins = play((struct GameTurn){ playerOnePosition, playerTwoPosition, 0, 0, true }, cachedWins);

        if (wins.playerOne > wins.playerTwo) {
            answer = wins.playerOne;
        } else {
            answer = wins.playerTwo;
        }
    }

    print(aoc, LONG, &answer);

    return 0;
}
