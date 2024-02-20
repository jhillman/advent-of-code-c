#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

enum GroupType {
    IMMUNE_SYSTEM,
    INFECTION
};

enum AttackType {
    BLUDGEONING,
    COLD,
    FIRE,
    RADIATION,
    SLASHING
};

enum AttackType parseAttackType(char *attack) {
    if (strstr(attack, "bludgeoning")) {
        return BLUDGEONING;
    } else if (strstr(attack, "cold")) {
        return COLD;
    } else if (strstr(attack, "fire")) {
        return FIRE;
    } else if (strstr(attack, "radiation")) {
        return RADIATION;
    } else if (strstr(attack, "slashing")) {
        return SLASHING;
    } else {
        printf("%s attack not found!\n", attack);
        exit(1);
    }
}

struct Attack {
    enum AttackType type;
    int damage;
    int initiative;
};

enum { IMMUNE, WEAK };

struct Group {
    int id;
    enum GroupType type;
    int units;
    int hitPoints;
    bool attacks[5][2];
    struct Attack attack;

    bool selected;
    struct Group *target;
};

int effectivePower(struct Group *group) {
    return group->units * group->attack.damage;
}

int compareEffectivePowerAndInitiative(const void *a, const void *b) {
    struct Group *groupA = *((struct Group **)a);
    struct Group *groupB = *((struct Group **)b);

    int result = effectivePower(groupB) - effectivePower(groupA);

    if (result == 0) {
        result = groupB->attack.initiative - groupA->attack.initiative;
    }

    return result;
}

int compareInitiative(const void *a, const void *b) {
    struct Group *groupA = *((struct Group **)a);
    struct Group *groupB = *((struct Group **)b);

    return groupB->attack.initiative - groupA->attack.initiative;
}

struct Groups {
    int capacity;
    int count;
    struct Group **data;
};

void addGroup(struct Groups *groups, struct Group group) {
    if (groups->count == groups->capacity) {
        groups->capacity += 10;
        groups->data = (struct Group **)realloc(groups->data, groups->capacity * sizeof(struct Group *));
    }

    groups->data[groups->count] = (struct Group *)calloc(1, sizeof(struct Group));
    *groups->data[groups->count++] = group;
}

struct Group *groupById(struct Groups *groups, int id) {
    struct Group *group = NULL;

    for (int i = 0; !group && i < groups->count; i++) {
        if (groups->data[i]->id == id) {
            group = groups->data[i];
        }
    }

    return group;
}

int attackDamage(struct Group *group, struct Group *target) {
    if (target->attacks[group->attack.type][IMMUNE]) {
        return 0;
    } else if (target->attacks[group->attack.type][WEAK]) {
        return effectivePower(group) * 2;
    } else {
        return effectivePower(group);
    }
}

struct Groups *getGroups(char *input, int boost) {
    FILE *inputFile = fopen(input, "r");
    struct Groups *groups = NULL;

    if (inputFile) {
        enum GroupType groupType;
        struct Group group;
        char line[256];
        int units;
        int hitPoints;
        char details[128];
        char *attackDetails;
        char *attackDetailTypes;
        char *attackDetailTypesContext;
        char *attackCategories;
        char *categoryAttackType;
        char *categoryAttackTypes;
        char attackType[16];
        int classification;
        int damage;
        int initiative;

        groups = (struct Groups *)calloc(1, sizeof(struct Groups));

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            if (strlen(line)) {
                if (strstr(line, "Immune System")) {
                    groupType = IMMUNE_SYSTEM;
                } else if (strstr(line, "Infection")) {
                    groupType = INFECTION;
                } else {
                    sscanf(line, "%d units each with %d hit points%[^0-9]%d %s damage at initiative %d", 
                        &units, &hitPoints, details, &damage, attackType, &initiative);

                    group = (struct Group){ 
                        groups->count,
                        groupType, 
                        units, 
                        hitPoints, 
                        { { false, false }, { false, false }, { false, false }, { false, false }, { false, false } },
                        {
                            parseAttackType(attackType),
                            damage + (groupType == IMMUNE ? boost : 0),
                            initiative
                        }
                    };

                    if ((attackDetails = strchr(details, '('))) {
                        ++attackDetails;

                        *strchr(attackDetails, ')') = '\0';

                        attackDetailTypes = strtok_r(attackDetails, ";", &attackDetailTypesContext);

                        while (attackDetailTypes) {
                            if (strstr(attackDetailTypes, "immune")) {
                                classification = IMMUNE;
                            } else if (strstr(attackDetailTypes, "weak")) {
                                classification = WEAK;
                            }

                            categoryAttackType = strtok(strstr(attackDetailTypes, " to ") + 4, ", ");

                            while (categoryAttackType) {
                                group.attacks[parseAttackType(categoryAttackType)][classification] = true;

                                categoryAttackType = strtok(NULL, ", ");
                            }

                            attackDetailTypes = strtok_r(NULL, ";", &attackDetailTypesContext);
                        }
                    }

                    addGroup(groups, group);
                }
            }
        }

        fclose(inputFile);
    }

    return groups;
}

void freeGroups(struct Groups *groups) {
    for (int i = 0; i < groups->count; i++) {
        free(groups->data[i]);
    }

    free(groups->data);
    free(groups);
}

void fight(struct Groups *groups) {
    struct Group *group;
    struct Group *target;
    int killedUnits;
    bool fighting = true;

    while (fighting) {
        fighting = false;

        qsort(groups->data, groups->count, sizeof(struct Group *), compareEffectivePowerAndInitiative);

        for (int i = 0; i < groups->count; i++) {
            groups->data[i]->selected = false;
            groups->data[i]->target = NULL;
        }

        for (int i = 0; i < groups->count; i++) {
            group = groups->data[i];

            if (group->units > 0) {
                for (int j = 0; j < groups->count; j++) {
                    target = groups->data[j];

                    if (target->type != group->type && target->units > 0 && !target->selected && attackDamage(group, target) > 0) {
                        if (group->target == NULL) {
                            group->target = target;
                            group->target->selected = true;                        
                        } else if (attackDamage(group, target) > attackDamage(group, group->target)) {
                            group->target->selected = false;
                            group->target = target;
                            group->target->selected = true;                        
                        }
                    }
                }
            }
        }

        qsort(groups->data, groups->count, sizeof(struct Group *), compareInitiative);

        for (int i = 0; i < groups->count; i++) {
            struct Group *group = groups->data[i];

            if (group->units > 0 && group->target && group->target->units > 0) {
                killedUnits = attackDamage(group, group->target) / group->target->hitPoints;

                if (killedUnits > 0) {
                    group->target->units -= killedUnits;
                    fighting = true;
                }
            }
        }
    }
}
