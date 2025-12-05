/* Day 5, part 1 = 782 */

#include "../../aoc.h"
#include "ingredients.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct IngredientData *data = getData(aoc.input);
    int answer = 0;

    if (data) {
        int freshCount = 0;

        for (int i = 0; i < data->ingredients->count; i++) {
            long ingredient = data->ingredients->data[i];
            bool fresh = false;

            for (int j = 0; !fresh && j < data->ranges->count; j++) {
                fresh = ingredient >= data->ranges->data[j].minimum && ingredient <= data->ranges->data[j].maximum;
            }

            if (fresh) {
                ++freshCount;
            }
        }

        answer = freshCount;

        freeData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
