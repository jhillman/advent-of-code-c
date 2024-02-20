#include "../../aoc.h"
#include <stdlib.h>

struct Marble {
    int value;
    struct Marble *clockwise;
    struct Marble *counterClockwise;
};

long play(char *input, long multiplier) {
    FILE *inputFile = fopen(input, "r");
    long maxScore = 0;

    if (inputFile) {
        int playerCount;
        int marbleCount;

        fscanf(inputFile, "%d players; last marble is worth %d points", &playerCount, &marbleCount);
        fclose(inputFile);

        marbleCount *= multiplier;

        struct Marble *marbles = (struct Marble *)calloc(marbleCount, sizeof(struct Marble));
        int marbleIndex = 0;

        struct Marble *marble = &marbles[marbleIndex++];

        long *scores = (long *)calloc(playerCount, sizeof(long));

        long points = 1;
        long player = 1;

        marble->value = 0;
        marble->clockwise = marble;
        marble->counterClockwise = marble;

        while (points <= marbleCount) {
            if (points % 23 == 0) {
                for (int i = 0; i < 7; i++) {
                    marble = marble->counterClockwise;
                }

                scores[player] += points + marble->value;

                if (scores[player] > maxScore) {
                    maxScore = scores[player];
                }

                struct Marble *clockwise = marble->clockwise;
                struct Marble *counterClockwise = marble->counterClockwise;

                clockwise->counterClockwise = counterClockwise;
                counterClockwise->clockwise = clockwise;

                marble = clockwise;
            } else {
                struct Marble *clockwise = marble->clockwise->clockwise;
                struct Marble *counterClockwise = marble->clockwise;
                
                marble = &marbles[marbleIndex++];
                marble->value = points;
                marble->clockwise = clockwise;
                marble->counterClockwise = counterClockwise;
                
                clockwise->counterClockwise = marble;
                counterClockwise->clockwise = marble;
            }

            ++points;
            player = (player + 1) % playerCount;
        }

        free(marbles);
        free(scores);
    }

    return maxScore;
}
