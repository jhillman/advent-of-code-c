/* Day 2, part 1 = 13052 */

#include "../../aoc.h"
#include "moves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char opponentMove;
        char myMove;
        int score = 0;

        while (fscanf(inputFile, "%c %c\n", &opponentMove, &myMove) == 2) {
            score += moveScore(myMove, 'X');
            score += (myMove == winningMove(opponentMove, 'X') ? 6 : (myMove == sameMove(opponentMove, 'X') ? 3 : 0));
        }

        fclose(inputFile);

        answer = score;
    }

    print(aoc, INT, &answer);

    return 0;
}
