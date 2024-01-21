/* Day 21, part 1 = 1815 */

#include "../../aoc.h"
#include "allergens.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct AllergenData *data = readAllergenData(aoc.input);
    int answer = 0;

    if (data) {
        int notAllergenIngredientCount = 0;
        bool notAllergenIngredient = true;

        for (int i = 0; i < data->ingredientListsCount; i++) {
            for (int j = 0; j < data->ingredientLists[i].count; j++) {
                notAllergenIngredient = true;

                for (int k = 0; notAllergenIngredient && k < data->allergenCount; k++) {
                    if (strcmp(*data->minimizedAllergenGroups[k].ingredientList.ingredients, data->ingredientLists[i].ingredients[j]) == 0) {
                        notAllergenIngredient = false;
                    }
                }

                if (notAllergenIngredient) {
                    ++notAllergenIngredientCount;
                }
            }
        }

        freeAllergenData(data);

        answer = notAllergenIngredientCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
