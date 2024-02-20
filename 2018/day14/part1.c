/* Day 14, part 1 = 1221283494 */

#include "recipes.h"

bool finished(struct RecipeData *data) {
    return data->iterations == data->input + 10;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct RecipeData *data = getRecipeData(aoc.input);
    long answer = 0;

    if (data) {
        setModulus(data, 10);

        while (!data->finished) {
            update(data, finished);
        }

        answer = data->recipes;

        freeRecipeData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
