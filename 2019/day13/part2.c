/* Day 13, part 2 = 17159 */

#include "../../aoc.h"
#include "game.h"
#include <time.h>
#include <errno.h>    

void msleep(long msec) {
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    while(nanosleep(&ts, &ts));
}

void printScreen(int **tiles, int score) {
    msleep(3);
    printf("\033[0;0H");

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            char tile;

            switch (tiles[y][x]) {
            case EMPTY:
                tile = ' ';
                break;
            case WALL:
                tile = '|';
                break;
            case BLOCK:
                tile = '#';
                break;
            case PADDLE:
                tile = '~';
                break;
            case BALL:
                tile = '*';
                break;
            }

            printf("%c", tile);
        }

        printf("\n");
    }

    printf("Score: %d\n", score);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    int answer = 0;

    if (program) {
        int **tiles = initializeTiles();

        program->haltValue = STOP_GAME;

        // quarters
        program->data[0] = 2;

        pipe(program->input);
        pipe(program->output);

        if (fork() == 0) {
            runProgram(program);

            exit(0);
        }

        long output;
        int score;
        int paddleX = 0;
        long paddlePosition;

        while (true) {
            read(program->output[READ], &output, sizeof(output));

            if (output == STOP_GAME) {
                break;
            }

            int x = (int)output;

            read(program->output[READ], &output, sizeof(output));

            if (output == STOP_GAME) {
                break;
            }

            int y = (int)output;

            read(program->output[READ], &output, sizeof(output));

            if (output == STOP_GAME) {
                break;
            }

            if (x == -1 && y == 0) {
                score = (int)output;
            } else {
                int tileId = (int)output;

                if (tileId == PADDLE) {
                    paddleX = x;
                } else if (tileId == BALL) {
                    if (paddleX < x) {
                        paddlePosition = 1;
                    } else if (paddleX > x) {
                        paddlePosition = -1;
                    } else {
                        paddlePosition = 0;
                    }

                    write(program->input[WRITE], &paddlePosition, sizeof(paddlePosition));
                }

                tiles[y][x] = tileId;
            }

            // printScreen(tiles, score);
        }

        freeProgram(program);
        freeTiles(tiles);

        answer = score;
    }

    print(aoc, INT, &answer);

    return 0;
}
