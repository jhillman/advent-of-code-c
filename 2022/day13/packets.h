#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

enum ListItemType {
    LIST,
    VALUE
};

struct ListItem {
    enum ListItemType type;
    struct ListItem *list;
    int itemCount;
    int value;
    bool divider;
};

enum ComparisonResult {
    LEFT,
    RIGHT,
    SAME
};

int numberLength(int number) {
    if (number == 0) {
        return 1;
    }

    return (int)floor(log10((double)abs(number))) + (number < 0 ? 2 : 1);
}

struct ListItem readValue(char *packet, int *valueLength) {
    struct ListItem value = { VALUE, NULL, 0, 0, false };
    sscanf(packet, "%d", &value.value);

    *valueLength = numberLength(value.value);

    return value;
}

struct ListItem readList(char *packet, int *listLength) {
    struct ListItem list = { LIST, NULL, 0, 0, false };
    int itemLength;
    bool listComplete = false;

    if (listLength) {
        *listLength = 1;
    }

    ++packet;

    while (!listComplete) {
        switch (*packet) {
        case '[':
            if (list.itemCount == 0) {
                list.list = (struct ListItem *)malloc(sizeof(struct ListItem));
            } else {
                list.list = (struct ListItem *)realloc(list.list, (list.itemCount + 1) * sizeof(struct ListItem));
            }

            list.list[list.itemCount++] = readList(packet, &itemLength);

            packet += itemLength;

            if (listLength) {
                *listLength += itemLength;
            }
            break;
        case ']':
            if (listLength) {
                *listLength += 1;
            }

            listComplete = true;
            break;
        case ',':
            if (listLength) {
                *listLength += 1;
            }

            ++packet;
            break;
        default:
            if (list.itemCount == 0) {
                list.list = (struct ListItem *)malloc(sizeof(struct ListItem));
            } else {
                list.list = (struct ListItem *)realloc(list.list, (list.itemCount + 1) * sizeof(struct ListItem));
            }

            list.list[list.itemCount++] = readValue(packet, &itemLength);

            packet += itemLength;

            if (listLength) {
                *listLength += itemLength;
            }
            break;
        }
    }

    return list;
}

struct ListItem makeList(int value) {
    struct ListItem list = { LIST, (struct ListItem *)malloc(sizeof(struct ListItem)), 1, 0 };
    *list.list = (struct ListItem){ VALUE, NULL, 0, value, false };

    return list;
}

void freeList(struct ListItem *list) {
    if (list) {
        for (int i = 0; i < list->itemCount; i++) {
            if (list->list[i].type == LIST) {
                freeList(list->list[i].list);
            }
        }

        free(list);
    }
}

enum ComparisonResult compare(struct ListItem left, struct ListItem right) {
    if (left.type == VALUE && right.type == VALUE) {
        if (left.value < right.value) {
            return LEFT;
        } else if (left.value > right.value) {
            return RIGHT;
        } else {
            return SAME;
        }
    } else if (left.type == LIST && right.type == LIST) {
        int index = 0;

        while (index < left.itemCount && index < right.itemCount) {
            enum ComparisonResult result = compare(left.list[index], right.list[index]);

            if (result == SAME) {
                ++index;
            } else {
                return result;
            }
        }

        if (index == left.itemCount && index < right.itemCount) {
            return LEFT;
        } else if (index == right.itemCount && index < left.itemCount) {
            return RIGHT;
        } else {
            return SAME;
        }
    } else if (left.type == LIST && right.type == VALUE) {
        struct ListItem list = makeList(right.value);

        enum ComparisonResult result = compare(left, list);

        freeList(list.list);

        return result;
    } else if (left.type == VALUE && right.type == LIST) {
        struct ListItem list = makeList(left.value);

        enum ComparisonResult result = compare(list, right);

        freeList(list.list);

        return result;
    }

    return SAME;
}

struct ListItem *getPackets(char *input, int *packetCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        *packetCount = 0;

        char packet[256];

        while (!feof(inputFile)) {
            fgets(packet, sizeof(packet), inputFile);

            if (*packet == '[') {
                ++*packetCount;
            }
        }

        struct ListItem *packets = (struct ListItem *)malloc(*packetCount * sizeof(struct ListItem));

        fseek(inputFile, 0, SEEK_SET);

        for (int i = 0; i < *packetCount; i++) {
            fgets(packet, sizeof(packet), inputFile);
            packets[i++] = readList(packet, NULL);

            fgets(packet, sizeof(packet), inputFile);
            packets[i] = readList(packet, NULL);

            fscanf(inputFile, "\n");
        }

        fclose(inputFile);

        return packets;
    }

    return NULL;
}