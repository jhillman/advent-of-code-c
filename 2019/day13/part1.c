/* Day 13, part 1 = 363 */

#include "../../aoc.h"
#include "game.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    int answer = 0;

    if (program) {
        int **tiles = initializeTiles();

        program->haltValue = STOP_GAME;

        pipe(program->output);

        if (fork() == 0) {
            runProgram(program);

            exit(0);
        }

        long output;

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

            int tileId = (int)output;

            tiles[y][x] = tileId;
        }

        freeProgram(program);

        int blockTileCount = 0;

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (tiles[y][x] == BLOCK) {
                    ++blockTileCount;
                }
            }
        }

        freeTiles(tiles);

        answer = blockTileCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
