/* Day 21, part 1 = 597600 */

#include "../../aoc.h"
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int playerOnePosition;
        int playerTwoPosition;
        int playerOneScore = 0;
        int playerTwoScore = 0;
        int diceValue = 1;
        int diceRolls = 0;
        int diceTotal;
        bool playerOneTurn = true;

        fscanf(inputFile, "Player 1 starting position: %d\n", &playerOnePosition);
        fscanf(inputFile, "Player 2 starting position: %d", &playerTwoPosition);

        fclose(inputFile);

        while (playerOneScore < 1000 && playerTwoScore < 1000) {
            diceTotal = diceValue + (diceValue + 1) % 100 + (diceValue + 2) % 100;
            diceRolls += 3;

            if (playerOneTurn) {
                if ((playerOnePosition = (playerOnePosition + diceTotal) % 10) == 0) {
                    playerOnePosition = 10;
                }

                playerOneScore += playerOnePosition;
            } else {
                if ((playerTwoPosition = (playerTwoPosition + diceTotal) % 10) == 0) {
                    playerTwoPosition = 10;
                }

                playerTwoScore += playerTwoPosition;
            }

            if (playerOneScore < 1000 && playerTwoScore < 1000) {
                diceValue += 3;

                if (diceValue > 100) {
                    if ((diceValue %= 100) == 0) {
                        diceValue = 1;
                    }
                }

                playerOneTurn = !playerOneTurn;
            }
        }

        if (playerOneTurn) {
            answer = playerTwoScore * diceRolls;
        } else {
            answer = playerOneScore * diceRolls;
        }
    }

    print(aoc, INT, &answer);

    return 0;
}
