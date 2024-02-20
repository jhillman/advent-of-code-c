/* Day 23, part 2 = 160646364 */

#include "bots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Bots *bots = getBots(aoc.input);
    int answer = 0;

    if (bots) {
        struct Space space = { { 0, 0, 0 }, { 0, 0, 0 } };
        struct Bot bot;

        for (int i = 0; i < bots->count; i++) {
            bot = bots->data[i];

            space.minimum.x = space.minimum.x ? min(space.minimum.x, bot.location.x) : bot.location.x;
            space.maximum.x = space.maximum.x ? max(space.maximum.x, bot.location.x) : bot.location.x;

            space.minimum.y = space.minimum.y ? min(space.minimum.y, bot.location.y) : bot.location.y;
            space.maximum.y = space.maximum.y ? max(space.maximum.y, bot.location.y) : bot.location.y;

            space.minimum.z = space.minimum.z ? min(space.minimum.z, bot.location.z) : bot.location.z;
            space.maximum.z = space.maximum.z ? max(space.maximum.z, bot.location.z) : bot.location.z;
        }

        calculateDetails(&space);

        struct BotStateQueue *queue = (struct BotStateQueue *)calloc(1, sizeof(struct BotStateQueue));
        struct BotState state = { bots, space };
        struct BotState next;
        struct XYZ middle;

        enqueue(queue, state);

        while (queue->size) {
            state = dequeue(queue);

            if (state.space.area == 1) {
                answer = state.space.distance;
                break;
            }

            middle = (struct XYZ){
                state.space.minimum.x + ((state.space.maximum.x - state.space.minimum.x) / 2), 
                state.space.minimum.y + ((state.space.maximum.y - state.space.minimum.y) / 2), 
                state.space.minimum.z + ((state.space.maximum.z - state.space.minimum.z) / 2)
            };

            struct Space spaces[] = {
                { 
                    { state.space.minimum.x, state.space.minimum.y, state.space.minimum.z }, 
                    { middle.x, middle.y, middle.z } 
                },
                { 
                    { middle.x + 1, state.space.minimum.y, state.space.minimum.z }, 
                    { state.space.maximum.x, middle.y, middle.z } 
                },
                { 
                    { state.space.minimum.x, middle.y + 1, state.space.minimum.z }, 
                    { middle.x, state.space.maximum.y, middle.z } 
                },
                { 
                    { middle.x + 1, middle.y + 1, state.space.minimum.z }, 
                    { state.space.maximum.x, state.space.maximum.y, middle.z } 
                },
                { 
                    { state.space.minimum.x, state.space.minimum.y, middle.z + 1 }, 
                    { middle.x, middle.y, state.space.maximum.z } 
                },
                { 
                    { middle.x + 1, state.space.minimum.y, middle.z + 1 }, 
                    { state.space.maximum.x, middle.y, state.space.maximum.z } 
                },
                { 
                    { state.space.minimum.x, middle.y + 1, middle.z + 1 }, 
                    { middle.x, state.space.maximum.y, state.space.maximum.z } 
                },
                { 
                    { middle.x + 1, middle.y + 1, middle.z + 1 }, 
                    { state.space.maximum.x, state.space.maximum.y, state.space.maximum.z } 
                }
            };

            for (int i = 0; i < 8; i++) {
                calculateDetails(&spaces[i]);

                next = (struct BotState){ (struct Bots *)calloc(1, sizeof(struct Bots)), spaces[i] };

                for (int j = 0; j < state.bots->count; j++) {
                    if (inRange(state.bots->data[j], spaces[i])) {
                        addBot(next.bots, state.bots->data[j]);
                    }
                }

                enqueue(queue, next);
            }

            freeBots(state.bots);
        }

        if (queue->size) {
            freeBots(state.bots);
        }

        freeQueue(queue);
    }

    print(aoc, INT, &answer);

    return 0;
}