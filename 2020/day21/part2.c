/* Day 21, part 2 = kllgt,jrnqx,ljvx,zxstb,gnbxs,mhtc,hfdxb,hbfnkq */

#include "../../aoc.h"
#include "allergens.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct AllergenData *data = readAllergenData(aoc.input);
    char answer[64] = { 0 };

    if (data) {
        int notAllergenIngredientCount = 0;

        for (int i = 0; i < data->allergenCount; i++) {
            sprintf(answer + strlen(answer), "%s", *data->minimizedAllergenGroups[i].ingredientList.ingredients);
            
            if (i < data->allergenCount - 1) {
                sprintf(answer + strlen(answer), ",");
            }
        }

        freeAllergenData(data);
    }

    print(aoc, STRING, answer);

    return 0;
}
