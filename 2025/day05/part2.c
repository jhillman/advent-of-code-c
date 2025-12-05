/* Day 5, part 2 = 353863745078671 */

#include "../../aoc.h"
#include "ingredients.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct IngredientData *data = getData(aoc.input);
    long answer = 0;

    if (data) {
        long freshCount = 0;

        for (int i = 0; i < data->ranges->count; i++) {
            if (i > 0 && data->ranges->data[i].minimum >= data->ranges->data[i - 1].minimum && 
                         data->ranges->data[i].maximum <= data->ranges->data[i - 1].maximum) {
                continue;
            } else if (i > 0 && data->ranges->data[i].minimum <= data->ranges->data[i - 1].maximum) {
                data->ranges->data[i].minimum = data->ranges->data[i - 1].maximum + 1;
            }

            freshCount += data->ranges->data[i].maximum - data->ranges->data[i].minimum + 1;
        }

        answer = freshCount;

        freeData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
