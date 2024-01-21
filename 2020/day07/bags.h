#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum Modifier {
    BRIGHT,
    CLEAR,
    DARK,
    DIM,
    DOTTED,
    DRAB,
    DULL,
    FADED,
    LIGHT,
    MIRRORED,
    MUTED,
    PALE,
    PLAID,
    POSH,
    SHINY,
    STRIPED,
    VIBRANT,
    WAVY
};

enum Modifier getModifier(char *modifier) {
    if (strcmp(modifier, "bright") == 0) {
        return BRIGHT;
    } else if (strcmp(modifier, "clear") == 0) {
        return CLEAR;
    } else if (strcmp(modifier, "dark") == 0) {
        return DARK;
    } else if (strcmp(modifier, "dim") == 0) {
        return DIM;
    } else if (strcmp(modifier, "dotted") == 0) {
        return DOTTED;
    } else if (strcmp(modifier, "drab") == 0) {
        return DRAB;
    } else if (strcmp(modifier, "dull") == 0) {
        return DULL;
    } else if (strcmp(modifier, "faded") == 0) {
        return FADED;
    } else if (strcmp(modifier, "light") == 0) {
        return LIGHT;
    } else if (strcmp(modifier, "mirrored") == 0) {
        return MIRRORED;
    } else if (strcmp(modifier, "muted") == 0) {
        return MUTED;
    } else if (strcmp(modifier, "pale") == 0) {
        return PALE;
    } else if (strcmp(modifier, "plaid") == 0) {
        return PLAID;
    } else if (strcmp(modifier, "posh") == 0) {
        return POSH;
    } else if (strcmp(modifier, "shiny") == 0) {
        return SHINY;
    } else if (strcmp(modifier, "striped") == 0) {
        return STRIPED;
    } else if (strcmp(modifier, "vibrant") == 0) {
        return VIBRANT;
    } else {
        return WAVY;
    }
}

void printModifier(enum Modifier modifier) {
    switch (modifier) {
    case BRIGHT:
        printf("bright");
        break;
    case CLEAR:
        printf("clear");
        break;
    case DARK:
        printf("dark");
        break;
    case DIM:
        printf("dim");
        break;
    case DOTTED:
        printf("dotted");
        break;
    case DRAB:
        printf("drab");
        break;
    case DULL:
        printf("dull");
        break;
    case FADED:
        printf("faded");
        break;
    case LIGHT:
        printf("light");
        break;
    case MIRRORED:
        printf("mirrored");
        break;
    case MUTED:
        printf("muted");
        break;
    case PALE:
        printf("pale");
        break;
    case PLAID:
        printf("plaid");
        break;
    case POSH:
        printf("posh");
        break;
    case SHINY:
        printf("shiny");
        break;
    case STRIPED:
        printf("striped");
        break;
    case VIBRANT:
        printf("vibrant");
        break;
    case WAVY:
        printf("wavy");
        break;
    }
}

enum Color {
    AQUA,
    BEIGE,
    BLACK,
    BLUE,
    BRONZE,
    BROWN,
    CHARTREUSE,
    CORAL,
    CRIMSON,
    CYAN,
    FUCHSIA,
    GOLD,
    GRAY,
    GREEN,
    INDIGO,
    LAVENDER,
    LIME,
    MAGENTA,
    MAROON,
    OLIVE,
    ORANGE,
    PLUM,
    PURPLE,
    RED,
    SALMON,
    SILVER,
    TAN,
    TEAL,
    TOMATO,
    TURQUOISE,
    VIOLET,
    WHITE,
    YELLOW
};

enum Color getColor(char *color) {
    if (strcmp(color, "aqua") == 0) {
        return AQUA;
    } else if (strcmp(color, "beige") == 0) {
        return BEIGE;
    } else if (strcmp(color, "black") == 0) {
        return BLACK;
    } else if (strcmp(color, "blue") == 0) {
        return BLUE;
    } else if (strcmp(color, "bronze") == 0) {
        return BRONZE;
    } else if (strcmp(color, "brown") == 0) {
        return BROWN;
    } else if (strcmp(color, "chartreuse") == 0) {
        return CHARTREUSE;
    } else if (strcmp(color, "coral") == 0) {
        return CORAL;
    } else if (strcmp(color, "crimson") == 0) {
        return CRIMSON;
    } else if (strcmp(color, "cyan") == 0) {
        return CYAN;
    } else if (strcmp(color, "fuchsia") == 0) {
        return FUCHSIA;
    } else if (strcmp(color, "gold") == 0) {
        return GOLD;
    } else if (strcmp(color, "gray") == 0) {
        return GRAY;
    } else if (strcmp(color, "green") == 0) {
        return GREEN;
    } else if (strcmp(color, "indigo") == 0) {
        return INDIGO;
    } else if (strcmp(color, "lavender") == 0) {
        return LAVENDER;
    } else if (strcmp(color, "lime") == 0) {
        return LIME;
    } else if (strcmp(color, "magenta") == 0) {
        return MAGENTA;
    } else if (strcmp(color, "maroon") == 0) {
        return MAROON;
    } else if (strcmp(color, "olive") == 0) {
        return OLIVE;
    } else if (strcmp(color, "orange") == 0) {
        return ORANGE;
    } else if (strcmp(color, "plum") == 0) {
        return PLUM;
    } else if (strcmp(color, "purple") == 0) {
        return PURPLE;
    } else if (strcmp(color, "red") == 0) {
        return RED;
    } else if (strcmp(color, "salmon") == 0) {
        return SALMON;
    } else if (strcmp(color, "silver") == 0) {
        return SILVER;
    } else if (strcmp(color, "tan") == 0) {
        return TAN;
    } else if (strcmp(color, "teal") == 0) {
        return TEAL;
    } else if (strcmp(color, "tomato") == 0) {
        return TOMATO;
    } else if (strcmp(color, "turquoise") == 0) {
        return TURQUOISE;
    } else if (strcmp(color, "violet") == 0) {
        return VIOLET;
    } else if (strcmp(color, "white") == 0) {
        return WHITE;
    } else {
        return YELLOW;
    }
}

void printColor(enum Color color) {
    switch (color) {
    case AQUA:
        printf("aqua");
        break;
    case BEIGE:
        printf("beige");
        break;
    case BLACK:
        printf("black");
        break;
    case BLUE:
        printf("blue");
        break;
    case BRONZE:
        printf("bronze");
        break;
    case BROWN:
        printf("brown");
        break;
    case CHARTREUSE:
        printf("chartreuse");
        break;
    case CORAL:
        printf("coral");
        break;
    case CRIMSON:
        printf("crimson");
        break;
    case CYAN:
        printf("cyan");
        break;
    case FUCHSIA:
        printf("fuchsia");
        break;
    case GOLD:
        printf("gold");
        break;
    case GRAY:
        printf("gray");
        break;
    case GREEN:
        printf("green");
        break;
    case INDIGO:
        printf("indigo");
        break;
    case LAVENDER:
        printf("lavender");
        break;
    case LIME:
        printf("lime");
        break;
    case MAGENTA:
        printf("magenta");
        break;
    case MAROON:
        printf("maroon");
        break;
    case OLIVE:
        printf("olive");
        break;
    case ORANGE:
        printf("orange");
        break;
    case PLUM:
        printf("plum");
        break;
    case PURPLE:
        printf("purple");
        break;
    case RED:
        printf("red");
        break;
    case SALMON:
        printf("salmon");
        break;
    case SILVER:
        printf("silver");
        break;
    case TAN:
        printf("tan");
        break;
    case TEAL:
        printf("teal");
        break;
    case TOMATO:
        printf("tomato");
        break;
    case TURQUOISE:
        printf("turquoise");
        break;
    case VIOLET:
        printf("violet");
        break;
    case WHITE:
        printf("white");
        break;
    case YELLOW:
        printf("yellow");
        break;
    }
}

struct Bag {
    enum Modifier modifier;
    enum Color color;
    int count;
};

struct Rule {
    struct Bag bag;
    struct Bag *bags;
    int bagCount;
    bool canContain;
};

struct RuleData {
    struct Rule *rules;
    int ruleCount;
    struct Rule ruleLookUp[WAVY - BRIGHT + 1][YELLOW - AQUA + 1];
};

struct RuleData readInput(char *input) {
    FILE *inputFile = fopen(input, "r");

    struct RuleData data;
    data.ruleCount = 0;

    if (inputFile) {
        char ch;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++data.ruleCount;
            }
        }

        if (ch != '\n' && data.ruleCount > 0) {
            ++data.ruleCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        data.rules = (struct Rule *)calloc(data.ruleCount, sizeof(struct Rule));

        char rulesLine[150];
        int ruleIndex = 0;
        char separator[] = "contain ";

        while (fgets(rulesLine, sizeof(rulesLine), inputFile)) {
            char modifier[12];
            char color[12];

            sscanf(rulesLine, "%s %s", modifier, color);

            data.rules[ruleIndex].bag.modifier = getModifier(modifier);
            data.rules[ruleIndex].bag.color = getColor(color);

            int bagCount = 0;
            char *bagsPtr = strstr(rulesLine, separator) + strlen(separator);

            if (!strstr(rulesLine, "contain no")) {
                while (*bagsPtr) {
                    if (*bagsPtr == ',' || *bagsPtr == '.') {
                        ++bagCount;
                    }

                    ++bagsPtr;
                }
            }

            data.rules[ruleIndex].bagCount = bagCount;
            data.rules[ruleIndex].canContain = false;

            if (bagCount > 0) {
                bagsPtr = strstr(rulesLine, separator) + strlen(separator);
                bagsPtr = strtok(bagsPtr, ",");
    
                int bagIndex = 0;
                data.rules[ruleIndex].bags = (struct Bag *)calloc(bagCount, sizeof(struct Bag));

                while (bagIndex < bagCount) {
                    int count;

                    sscanf(bagsPtr, "%d %s %s%c", &count, modifier, color, &ch);

                    data.rules[ruleIndex].bags[bagIndex].modifier = getModifier(modifier);
                    data.rules[ruleIndex].bags[bagIndex].color = getColor(color);

                    data.rules[ruleIndex].bags[bagIndex++].count = count;

                    bagsPtr = strtok(NULL, ",");
                }
            }

            data.ruleLookUp[data.rules[ruleIndex].bag.modifier][data.rules[ruleIndex].bag.color] = data.rules[ruleIndex];

            ++ruleIndex;
        }

        fclose(inputFile);
    }

    return data;
}

void freeData(struct RuleData data) {
    for (int i = 0; i < data.ruleCount; i++) {
        free(data.rules[i].bags);
    }

    free(data.rules);
}
