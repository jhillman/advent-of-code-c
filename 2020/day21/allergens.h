#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct IngredientList {
    char **ingredients;
    int count;
};

struct AllergenGroup {
    char name[10];
    struct IngredientList ingredientList;
};

struct AllergenData {
    struct AllergenGroup *allergenGroups;
    int allergenGroupCount;
    struct IngredientList *ingredientLists;
    int ingredientListsCount;
    struct AllergenGroup *minimizedAllergenGroups;
    int allergenCount;
};

int compare(const void *a, const void *b) {
    return strcmp(((struct AllergenGroup *)a)->name, ((struct AllergenGroup *)b)->name);
}

char **intersection(char **ingredients1, int ingredientCount1, char **ingredients2, int ingredientCount2, int *intersectionCount) {
    char **intersectedIngredients = NULL;

    if (ingredients1 && ingredients2) {
        *intersectionCount = 0;

        for (int i = 0; i < ingredientCount1; i++) {
            char *ingredient1 = ingredients1[i];

            for (int j = 0; j < ingredientCount2; j++) {
                char *ingredient2 = ingredients2[j];

                if (strcmp(ingredient1, ingredient2) == 0) {
                    ++*intersectionCount;

                    if (intersectedIngredients) {
                        intersectedIngredients = (char **) realloc(intersectedIngredients, *intersectionCount * sizeof(char *));
                    } else {
                        intersectedIngredients = (char **) malloc(*intersectionCount * sizeof(char *));
                    }

                    intersectedIngredients[*intersectionCount - 1] = ingredient1;
                }
            }
        }

        free(ingredients1);
    } else {
        *intersectionCount = ingredientCount1;
        intersectedIngredients = (char **) malloc(*intersectionCount * sizeof(char *));

        for (int i = 0; i < ingredientCount1; i++) {
            intersectedIngredients[i] = ingredients1[i];
        }
    }

    return intersectedIngredients;
}

void removeIngredient(struct AllergenGroup *allergenGroups, int allergenCount, char *ingredient, char *allergen) {
    for (int i = 0; i < allergenCount; i++) {
        if (strcmp(allergenGroups[i].name, allergen) != 0 && allergenGroups[i].ingredientList.count > 1) {
            bool ingredientRemoved = false;

            for (int j = 0; j < allergenGroups[i].ingredientList.count; j++) {
                if (strcmp(allergenGroups[i].ingredientList.ingredients[j], ingredient) == 0) {
                    allergenGroups[i].ingredientList.ingredients[j] = NULL;
                    ingredientRemoved = true;
                } else if (ingredientRemoved) {
                    allergenGroups[i].ingredientList.ingredients[j - 1] = allergenGroups[i].ingredientList.ingredients[j];
                }
            }

            if (ingredientRemoved) {
                if (--allergenGroups[i].ingredientList.count == 1) {
                    removeIngredient(allergenGroups, allergenCount, *allergenGroups[i].ingredientList.ingredients, allergenGroups[i].name);
                }
            }
        }
    }
}

struct AllergenData *readAllergenData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct AllergenData *data = (struct AllergenData *) calloc(1, sizeof(struct AllergenData));

        int allergenGroupSize = 20;
        data->allergenGroupCount = 0;
        data->allergenGroups = (struct AllergenGroup *) malloc(allergenGroupSize * sizeof(struct AllergenGroup));

        int ingredientListsSize = 10;
        data->ingredientListsCount = 0;
        data->ingredientLists = (struct IngredientList *) malloc(ingredientListsSize * sizeof(struct IngredientList));

        char *line = (char *) malloc(600 * sizeof(char));
        size_t length;

        while (getline(&line, &length, inputFile) != -1) {
            char *ingredients = strtok(line, "(");
            char *allergens = strtok(NULL, "(") + 9;

            int ingredientCount = 0;
            char *charPtr = ingredients;

            while (*charPtr) {
                if (*charPtr == ' ') {
                    ++ingredientCount;
                }

                ++charPtr;
            }

            data->ingredientLists[data->ingredientListsCount].ingredients = (char **) calloc(ingredientCount, sizeof(char *));
            data->ingredientLists[data->ingredientListsCount].count = ingredientCount;
            int ingredentIndex = 0;

            char *ingredient = strtok(ingredients, " ");

            while (ingredient) {
                data->ingredientLists[data->ingredientListsCount].ingredients[ingredentIndex] = (char *) calloc(strlen(ingredient) + 1, sizeof(char));
                strcpy(data->ingredientLists[data->ingredientListsCount].ingredients[ingredentIndex++], ingredient);

                ingredient = strtok(NULL, " ");
            }

            char *allergen = strtok(allergens, " ,)");

            while (allergen) {
                if (strlen(allergen) > 1) {
                    strcpy(data->allergenGroups[data->allergenGroupCount].name, allergen);
                    data->allergenGroups[data->allergenGroupCount].ingredientList.ingredients = data->ingredientLists[data->ingredientListsCount].ingredients;
                    data->allergenGroups[data->allergenGroupCount].ingredientList.count = ingredientCount;

                    ++data->allergenGroupCount;
                }

                allergen = strtok(NULL, " ,)");
            }

            ++data->ingredientListsCount;

            if (data->allergenGroupCount >= allergenGroupSize - 5) {
                allergenGroupSize *= 1.5;
                data->allergenGroups = (struct AllergenGroup *) realloc(data->allergenGroups, allergenGroupSize * sizeof(struct AllergenGroup));
            }

            if (data->ingredientListsCount >= ingredientListsSize - 5) {
                ingredientListsSize *= 1.5;
                data->ingredientLists = (struct IngredientList *) realloc(data->ingredientLists, ingredientListsSize * sizeof(struct IngredientList));
            }
        }

        fclose(inputFile);

        qsort(data->allergenGroups, data->allergenGroupCount, sizeof(struct AllergenGroup), compare);

        char *allergen = NULL;
        data->allergenCount = 0;

        for (int i = 0; i < data->allergenGroupCount; i++) {
            if (allergen == NULL || strcmp(data->allergenGroups[i].name, allergen) != 0) {
                allergen = data->allergenGroups[i].name;
                ++data->allergenCount;
            }
        }

        data->minimizedAllergenGroups = (struct AllergenGroup *) calloc(data->allergenCount, sizeof(struct AllergenGroup));
        int allergenGroupIndex = -1;

        allergen = NULL;
        char *singleIngredient = NULL;
        char *singleIngredientAllergen = NULL;

        for (int i = 0; i < data->allergenGroupCount; i++) {
            if (allergen == NULL || strcmp(data->allergenGroups[i].name, allergen) != 0) {
                ++allergenGroupIndex;
                allergen = data->allergenGroups[i].name;
                strcpy(data->minimizedAllergenGroups[allergenGroupIndex].name, allergen);
                data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.ingredients = intersection(
                    data->allergenGroups[i].ingredientList.ingredients, 
                    data->allergenGroups[i].ingredientList.count, 
                    NULL, 
                    0,
                    &data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.count);
            } else {
                data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.ingredients = intersection(
                    data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.ingredients,
                    data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.count, 
                    data->allergenGroups[i].ingredientList.ingredients, 
                    data->allergenGroups[i].ingredientList.count,
                    &data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.count);

                if (data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.count == 1) {
                    singleIngredient = *data->minimizedAllergenGroups[allergenGroupIndex].ingredientList.ingredients;
                    singleIngredientAllergen = data->minimizedAllergenGroups[allergenGroupIndex].name;
                }
            }
        }

        if (singleIngredient && singleIngredientAllergen) {
            removeIngredient(data->minimizedAllergenGroups, data->allergenCount, singleIngredient, singleIngredientAllergen);
        }

        return data;
    }

    return NULL;
}

void freeAllergenData(struct AllergenData *data) {
    for (int i = 0; i < data->allergenCount; i++) {
        free(data->minimizedAllergenGroups[i].ingredientList.ingredients);
    }

    free(data->minimizedAllergenGroups);

    for (int i = 0; i < data->ingredientListsCount; i++) {
        free(data->ingredientLists[i].ingredients);
    }

    free(data->ingredientLists);
    free(data->allergenGroups);
}
