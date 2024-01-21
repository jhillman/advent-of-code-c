#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

struct Chemical {
    int id;
    char name[8];
};

struct Chemicals {
    int capacity;
    int count;
    struct Chemical *data;
};

void addChemical(struct Chemicals *chemicals, char name[8]) {
    if (chemicals->count == chemicals->capacity) {
        chemicals->capacity += 5;
        chemicals->data = (struct Chemical *)realloc(chemicals->data, chemicals->capacity * sizeof(struct Chemical));
    }

    struct Chemical chemical;
    chemical.id = chemicals->count;
    strcpy(chemical.name, name);

    chemicals->data[chemicals->count++] = chemical;
}

int getChemicalId(struct Chemicals *chemicals, char name[8]) {
    bool idFound = false;
    int id;

    for (int i = 0; !idFound && i < chemicals->count; i++) {
        if (strcmp(chemicals->data[i].name, name) == 0) {
            id = chemicals->data[i].id;
            idFound = true;
        }
    }

    if (!idFound) {
        id = chemicals->count;

        addChemical(chemicals, name);
    }

    return id;
}

struct ChemicalCount {
    int chemicalId;
    int count;
};

struct ChemicalCounts {
    int capacity;
    int count;
    struct ChemicalCount *data;
};

void addChemicalCount(struct ChemicalCounts *counts, struct ChemicalCount count) {
    if (counts->count == counts->capacity) {
        counts->capacity += 5;
        counts->data = (struct ChemicalCount *)realloc(counts->data, counts->capacity * sizeof(struct ChemicalCount));
    }

    counts->data[counts->count++] = count;
}

struct Reaction {
    struct ChemicalCounts *inputs;
    struct ChemicalCount output;
};

struct Reactions {
    int capacity;
    int count;
    struct Reaction *data;
};

void addReaction(struct Reactions *reactions, struct Reaction reaction) {
    if (reaction.output.chemicalId >= reactions->capacity) {
        reactions->capacity = reaction.output.chemicalId + 1;
        reactions->data = (struct Reaction *)realloc(reactions->data, reactions->capacity * sizeof(struct Reaction));
    }

    reactions->data[reaction.output.chemicalId] = reaction;
}

struct Stoichiometry {
    struct Reactions *reactions;
    struct Chemicals *chemicals;
    int oreId;
    int fuelId;
};

struct Stoichiometry *getStoichiometry(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Stoichiometry *stoichiometry;

    if (inputFile) {
        char line[128];
        struct ChemicalCounts *inputs;
        struct ChemicalCount count;
        char *input;
        char *reactionContext;
        char *inputContext;
        char name[8];
        int answer = 0;

        stoichiometry = (struct Stoichiometry *)calloc(1, sizeof(struct Stoichiometry));
        stoichiometry->reactions = (struct Reactions *)calloc(1, sizeof(struct Reactions));
        stoichiometry->chemicals = (struct Chemicals *)calloc(1, sizeof(struct Chemicals));

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            inputs = (struct ChemicalCounts *)calloc(1, sizeof(struct ChemicalCounts));
            char *chemicalDetails = strtok_r(strtok_r(line, "=>", &reactionContext), ",", &inputContext);

            while (chemicalDetails) {

                sscanf(chemicalDetails, "%d %s", &count.count, name);
                count.chemicalId = getChemicalId(stoichiometry->chemicals, name); 

                if (strcmp(name, "ORE") == 0) {
                    stoichiometry->oreId = count.chemicalId;
                }

                addChemicalCount(inputs, count);

                chemicalDetails = strtok_r(NULL, ",", &inputContext);
            }

            chemicalDetails = strtok_r(NULL, "=>", &reactionContext);

            sscanf(chemicalDetails, "%d %s", &count.count, name);
            count.chemicalId = getChemicalId(stoichiometry->chemicals, name); 

            if (strcmp(name, "FUEL") == 0) {
                stoichiometry->fuelId = count.chemicalId;
            }

            addReaction(stoichiometry->reactions, (struct Reaction){ inputs, count });
        }

        fclose(inputFile);
    }

    return stoichiometry;
}

void freeStoichiometry(struct Stoichiometry *stoichiometry) {
    for (int i = 0; i < stoichiometry->reactions->count; i++) {
        free(stoichiometry->reactions->data[i].inputs->data);
        free(stoichiometry->reactions->data[i].inputs);
    }

    free(stoichiometry->reactions->data);
    free(stoichiometry->reactions);

    free(stoichiometry->chemicals->data);
    free(stoichiometry->chemicals);

    free(stoichiometry);
}

long countOre(struct Stoichiometry *stoichiometry, long outputId, long amount, long *ingredientSurplus) {
    long count = 0;
    bool freeSurplus = false;

    if (!ingredientSurplus) {
         ingredientSurplus = (long *)calloc(stoichiometry->reactions->count, sizeof(long));
         freeSurplus = true;
    }

    struct Reaction reaction = stoichiometry->reactions->data[outputId];
    double existing = ingredientSurplus[reaction.output.chemicalId];
    long multiple = ceil(max(0, amount - existing) / reaction.output.count);
    long extra = (reaction.output.count * multiple) - (amount - existing);

    if (reaction.output.chemicalId != stoichiometry->oreId) {
        ingredientSurplus[reaction.output.chemicalId] = extra;
    }

    for (int i = 0; i < reaction.inputs->count; i++) {
        struct ChemicalCount input = reaction.inputs->data[i];

        if (input.chemicalId == stoichiometry->oreId) {
            count += multiple * input.count;
        } else {
            count += countOre(stoichiometry, input.chemicalId, multiple * input.count, ingredientSurplus);
        }
    }

    if (freeSurplus) {
        free(ingredientSurplus);
    }

    return count;
}
