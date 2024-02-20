/* Day 14, part 2 = 20261485 */

#include "recipes.h"

bool finished(struct RecipeData *data) {
    return data->recipes == data->input;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct RecipeData *data = getRecipeData(aoc.input);
    long answer = 0;

    if (data) {
        int digits = digitCount(data->input);

        setModulus(data, digits);

        while (!data->finished) {
            update(data, finished);
        }

        answer = data->iterations - digits;

        freeRecipeData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
