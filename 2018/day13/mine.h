#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

enum IntersectionAction {
    TURN_LEFT,
    CONTINUE_STRAIGHT,
    TURN_RIGHT
};

enum IntersectionAction nextAction(enum IntersectionAction action) {
    switch (action) {
    case TURN_LEFT:
        return CONTINUE_STRAIGHT;
    case CONTINUE_STRAIGHT:
        return TURN_RIGHT;
    case TURN_RIGHT:
        return TURN_LEFT;
    }
}

struct XY {
    int x;
    int y;
};

struct Cart {
    struct XY location;
    struct XY movement;

    enum IntersectionAction action; 

    bool crashed;
};

int compare(const void *a, const void *b) {
    struct Cart *cartA = (struct Cart *)a;
    struct Cart *cartB = (struct Cart *)b;

    int result = cartA->location.y - cartB->location.y;

    if (result == 0) {
        result = cartA->location.x - cartB->location.x;
    }

    return result;
}

struct Carts {
    int capacity;
    int count;
    struct Cart *data;
};

void addCart(struct Carts *carts, struct Cart cart) {
    if (carts->count == carts->capacity) {
        carts->capacity += 10;
        carts->data = (struct Cart *)realloc(carts->data, carts->capacity * sizeof(struct Cart));
    }

    carts->data[carts->count++] = cart;
}

void freeCarts(struct Carts *carts) {
    free(carts->data);
    free(carts);
}

struct Mine {
    int width;
    int height;
    char **tracks;

    struct Carts *carts;
    int movingCarts;
};

void moveCarts(struct Mine *mine) {
    struct Cart *cart;

    qsort(mine->carts->data, mine->carts->count, sizeof(struct Cart), compare);

    for (int i = 0; i < mine->carts->count; i++) {
        cart = &mine->carts->data[i];

        if (cart->crashed) {
            continue;
        }

        cart->location.x += cart->movement.x;
        cart->location.y += cart->movement.y;

        switch (mine->tracks[cart->location.y][cart->location.x]) {
        case '/':
            if (cart->movement.y) {
                cart->movement.x = -cart->movement.y;
                cart->movement.y = 0;
            } else {
                cart->movement.y = -cart->movement.x;
                cart->movement.x = 0;
            }
            break;
        case '\\':
            if (cart->movement.y) {
                cart->movement.x = cart->movement.y;
                cart->movement.y = 0;
            } else {
                cart->movement.y = cart->movement.x;
                cart->movement.x = 0;
            }
            break;
        case '+':
            switch (cart->action) {
            case TURN_LEFT:
                if (cart->movement.y) {
                    cart->movement.x = cart->movement.y < 0 ? -1 : 1;
                    cart->movement.y = 0;
                } else {
                    cart->movement.y = cart->movement.x < 0 ? 1 : -1;
                    cart->movement.x = 0;
                }
                break;
            case TURN_RIGHT:
                if (cart->movement.y) {
                    cart->movement.x = cart->movement.y < 0 ? 1 : -1;
                    cart->movement.y = 0;
                } else {
                    cart->movement.y = cart->movement.x < 0 ? -1 : 1;
                    cart->movement.x = 0;
                }
                break;
            default: 
                break;
            }

            cart->action = nextAction(cart->action);
            break;
        }

        for (int j = 0; j < mine->carts->count; j++) {
            if (j == i) {
                continue;
            }

            if (mine->carts->data[j].crashed) {
                continue;
            }

            if (mine->carts->data[j].location.x == cart->location.x && mine->carts->data[j].location.y == cart->location.y) {
                cart->crashed = true;
                mine->carts->data[j].crashed = true;

                mine->movingCarts -= 2;
            }            
        }
    }
}

void printMine(struct Mine *mine) {
    bool cartFound;

    for (int y = 0; y < mine->height; y++) {
        for (int x = 0; x < mine->width; x++) {
            cartFound = false;

            for (int i = 0; !cartFound && i < mine->carts->count; i++) {
                struct Cart *cart = &mine->carts->data[i];

                if (cart->location.x == x && cart->location.y == y) {
                    cartFound = true;

                    if (cart->movement.y) {
                        printf("%c", (cart->movement.y < 0 ? '^' : 'v'));
                    } else {
                        printf("%c", (cart->movement.x < 0 ? '<' : '>'));
                    }
                }
            }

            if (!cartFound) {
                printf("%c", mine->tracks[y][x]);
            }
        }

        printf("\n");
    }
}

void freeMine(struct Mine *mine) {
    for (int y = 0; y < mine->height; y++) {
        free(mine->tracks[y]);
    }

    free(mine->tracks);

    freeCarts(mine->carts);
}

struct Mine *getMine(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Mine *mine = NULL;

    if (inputFile) {
        char c;

        mine = (struct Mine *)calloc(1, sizeof(struct Mine));
        mine->carts = (struct Carts *)calloc(1, sizeof(struct Carts));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++mine->height;
            }

            if (!mine->height) {
                ++mine->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        mine->tracks = (char **)malloc(mine->height * sizeof(char *));

        for (int y = 0; y < mine->height; y++) {
            mine->tracks[y] = (char *)malloc(mine->width * sizeof(char));

            for (int x = 0; x < mine->width; x++) {
                c = fgetc(inputFile);

                switch (c) {
                case '^':
                case 'v':
                case '<':
                case '>':
                    ++mine->movingCarts;

                    switch (c) {
                    case '^':
                        addCart(mine->carts, (struct Cart){ { x, y }, { 0, -1 } });
                        c = '|';
                        break;
                    case 'v':
                        addCart(mine->carts, (struct Cart){ { x, y }, { 0, 1 } });
                        c = '|';
                        break;
                    case '<':
                        addCart(mine->carts, (struct Cart){ { x, y }, { -1, 0 } });
                        c = '-';
                        break;
                    case '>':
                        addCart(mine->carts, (struct Cart){ { x, y }, { 1, 0 } });
                        c = '-';
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }

                mine->tracks[y][x] = c;
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return mine;    
}