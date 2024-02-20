/* Day 22, part 2 = 1032 */

#include "cave.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cave *cave = getCave(aoc.input);
    int answer = 0;

    if (cave) {
        int yDeltas[] = {-1, 0, 1, 0};
        int xDeltas[] = {0, -1, 0, 1};
        struct SearchState state;
        struct Location step;
        enum Tool tool;
        int time;

        cave->queue = (struct SearchStateQueue *)calloc(1, sizeof(struct SearchStateQueue));
        cave->visited = (bool ***)malloc(cave->height * sizeof(bool **));

        for (int y = 0; y < cave->height; y++) {
            cave->visited[y] = (bool **)malloc(cave->width * sizeof(bool *));

            for (int x = 0; x < cave->width; x++) {
                cave->visited[y][x] = (bool *)calloc(3, sizeof(bool));
            }
        }

        enqueue(cave->queue, (struct SearchState){ cave->mouth, TORCH, 0 });

        while (cave->queue->size) {
            state = dequeue(cave->queue);

            if (state.location.x == cave->target.x && state.location.y == cave->target.y && state.tool == TORCH) {
                answer = state.time;
                break;
            }

            if (cave->visited[state.location.y][state.location.x][state.tool]) {
                continue;
            }

            cave->visited[state.location.y][state.location.x][state.tool] = true;

            tool = changeTool(cave, state);

            if (!cave->visited[state.location.y][state.location.x][tool]) {
                time = state.time + 7;

                enqueue(cave->queue, (struct SearchState){ 
                    state.location, 
                    tool, 
                    time,
                    time + timeBetween(state.location, cave->target)
                });
            }

            for (int i = 0; i < 4; i++) {
                step = (struct Location){ state.location.x + xDeltas[i], state.location.y + yDeltas[i] };
    
                if (step.x < 0 || step.y < 0) {
                    continue;
                }

                if (validTool(regionType(cave, step), state.tool)) {
                    time = state.time + 1;

                    enqueue(cave->queue, (struct SearchState){ 
                        step, 
                        state.tool, 
                        time,
                        time + timeBetween(step, cave->target)
                    });
                }
            }
        }

        freeCave(cave);
    }

    print(aoc, INT, &answer);

    return 0;
}
