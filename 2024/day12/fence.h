#include <stdlib.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Position {
    int x;
    int y;
    enum Direction direction;
};

int compare(const void *a, const void *b) {
    struct Position *positionA = (struct Position *)a;
    struct Position *positionB = (struct Position *)b;

    int result = positionA->y - positionB->y;

    if (result == 0) {
        result = positionA->x - positionB->x;
    }

    if (result == 0) {
        result = positionA->direction - positionB->direction;
    }

    return result;
}

struct Positions {
    int capacity;
    int count;
    struct Position *data;
};

void addPosition(struct Positions *positions, struct Position position) {
    if (positions->count == positions->capacity) {
        positions->capacity += 100;
        positions->data = (struct Position *)realloc(positions->data, positions->capacity * sizeof(struct Position));
    }

    positions->data[positions->count++] = position;
}

struct Region {
    char type;
    struct Positions *perimeter;
    struct Position minimum;
    struct Position maximum;
    int area;
    int sides;
};

struct Regions {
    int capacity;
    int count;
    struct Region *data;
};

void addRegion(struct Regions *regions, struct Region region) {
    if (regions->count == regions->capacity) {
        regions->capacity += 100;
        regions->data = (struct Region *)realloc(regions->data, regions->capacity * sizeof(struct Region));
    }

    regions->data[regions->count++] = region;
}

struct Map {
    int width;
    int height;
    char **grid;
    bool **visited;
    bool ***perimeterVisited;
    struct Regions *regions;
};

void getRegion(struct Map *map, struct Region *region, struct Position position, bool discount) {
    int xDeltas[] = { 0, 0, -1, 1 };
    int yDeltas[] = { -1, 1, 0, 0 };
    enum Direction directions[] = { UP, DOWN, LEFT, RIGHT };

    if (region == NULL) {
        struct Region region = { 
            map->grid[position.y][position.x], 
            (struct Positions *)calloc(1, sizeof(struct Positions)),
            { map->width + 2, map->height + 2 }, { -2, -2 },
            0, 0 
        };

        getRegion(map, &region, position, discount);

        if (discount) {
            qsort(region.perimeter->data, region.perimeter->count, sizeof(struct Position), compare);

            for (int i = 0; i < region.perimeter->count; i++) {
                bool found = false;
                struct Position perimeter = region.perimeter->data[i];

                for (int j = 0; !found && j < 4; j++) {
                    struct Position position = {
                        perimeter.x + xDeltas[j],
                        perimeter.y + yDeltas[j],
                        perimeter.direction
                    };

                    found = map->perimeterVisited[position.y + 2][position.x + 2][position.direction];
                }

                if (!found) {
                    ++region.sides;
                }

                map->perimeterVisited[perimeter.y + 2][perimeter.x + 2][perimeter.direction] = true;
            }

            for (int i = 0; i < region.perimeter->count; i++) {
                struct Position perimeter = region.perimeter->data[i];
                map->perimeterVisited[perimeter.y + 2][perimeter.x + 2][perimeter.direction] = false;
            }
        }

        addRegion(map->regions, region);
    } else if (!map->visited[position.y][position.x]) {
        ++region->area;
        map->visited[position.y][position.x] = true;

        for (int i = 0; i < 4; i++) {
            struct Position next = { 
                position.x + xDeltas[i], 
                position.y + yDeltas[i], 
                directions[i] 
            };

            if (next.x < 0 || next.x >= map->width || 
                next.y < 0 || next.y >= map->height || 
                map->grid[next.y][next.x] != region->type) {
                addPosition(region->perimeter, next);

           } else {
                getRegion(map, region, next, discount);
            }
        }
    }
}

int price(char *input, bool discount) {
    FILE *inputFile = fopen(input, "r");
    int price = 0;

    if (inputFile) {
        struct Map *map = (struct Map *)calloc(1, sizeof(struct Map));
        map->regions = (struct Regions *)calloc(1, sizeof(struct Regions *));

        char c;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++map->height;
            }

            if (!map->height) {
                ++map->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        map->grid = (char **)malloc(map->height * sizeof(char *));
        map->visited = (bool **)malloc(map->height * sizeof(bool *));

        if (discount) {
            map->perimeterVisited = (bool ***)malloc((map->height + 4) * sizeof(bool **));
        }

        for (int y = 0; y < map->height + 4; y++) {
            if (y < map->height) {
                map->grid[y] = (char *)calloc(map->width, sizeof(char));
                map->visited[y] = (bool *)calloc(map->width, sizeof(bool));
            }

            if (discount) {
                map->perimeterVisited[y] = (bool **)malloc((map->width + 4) * sizeof(bool *));
            }

            for (int x = 0; x < map->width + 4; x++) {
                if (y < map->height && x < map->width) {
                    map->grid[y][x] = fgetc(inputFile);
                }

                if (discount) {
                    map->perimeterVisited[y][x] = (bool *)calloc(5, sizeof(bool));
                }
            }

            if (y < map->height) {
                fgetc(inputFile); // \n
            }
        }

        fclose(inputFile);

        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                if (map->visited[y][x]) {
                    continue;
                }

                getRegion(map, NULL, (struct Position){ x, y }, discount);
            }
        }

        for (int i = 0; i < map->regions->count; i++) {
            price += map->regions->data[i].area * (discount ? map->regions->data[i].sides : map->regions->data[i].perimeter->count);
        }

        for (int y = 0; y < map->height + 4; y++) {
            if (y < map->height) {
                free(map->grid[y]);
                free(map->visited[y]);
            }

            for (int x = 0; discount && x < map->width + 4; x++) {
                free(map->perimeterVisited[y][x]);
            }
        }

        free(map->grid);
        free(map->visited);

        if (discount) {
            free(map->perimeterVisited);
        }

        free(map->regions->data);
        free(map->regions);

        free(map);
    }

    return price;
}
