/* Day 2, part 2 = 13693 */

#include "../../aoc.h"
#include "moves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char opponentMove;
        char strategy;
        char myMove;
        int score = 0;

        while (fscanf(inputFile, "%c %c\n", &opponentMove, &strategy) == 2) {
            switch (strategy) {
            case 'X':
                myMove = losingMove(opponentMove, 'A');
                break;
            case 'Y':
                myMove = sameMove(opponentMove, 'A');
                break;
            case 'Z':
                myMove = winningMove(opponentMove, 'A');
                break;
            }

            score += moveScore(myMove, 'A');
            score += (myMove == winningMove(opponentMove, 'A') ? 6 : (myMove == sameMove(opponentMove, 'A') ? 3 : 0));
        }

        fclose(inputFile);

        answer = score;
    }

    print(aoc, INT, &answer);

    return 0;
}
