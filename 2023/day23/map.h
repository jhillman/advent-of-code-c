#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

struct Location {
    int x;
    int y;
    int distance;
};

struct Locations {
    int capacity;
    int count;
    struct Location *data;
};

void addLocation(struct Locations *locations, struct Location location) {
    if (locations->count == locations->capacity) {
        locations->capacity += 10;
        locations->data = (struct Location *)realloc(locations->data, locations->capacity * sizeof(struct Location));
    }

    locations->data[locations->count++] = location;
}

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

enum TerrainType {
    PATH,
    FOREST,
    SLOPE_UP,
    SLOPE_DOWN,
    SLOPE_LEFT,
    SLOPE_RIGHT
};

struct MapTerrain {
    enum TerrainType type;
    struct Locations *accessibleIntersections;
};

struct Map {
    int width;
    int height;
    
    struct Location start;
    struct Location end;

    struct MapTerrain **terrain;
    bool **visited;
};

enum HikeType {
    ICY,
    DRY
};

void freeMap(struct Map *map) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (map->terrain[y][x].accessibleIntersections) {
                free(map->terrain[y][x].accessibleIntersections->data);
                free(map->terrain[y][x].accessibleIntersections);
            }
        }

        free(map->terrain[y]);
        free(map->visited[y]);
    }

    free(map->terrain);
    free(map->visited);

    free(map);
}

int longestIcyHike(struct Map *map, struct Location location, int hikeLength) {
    int longest = 0;

    if (!(location.x >= 0 && location.x < map->width && location.y >= 0 && location.y < map->height)) {
        return 0;
    }

    if (map->terrain[location.y][location.x].type == FOREST) {
        return 0;
    }

    if (map->visited[location.y][location.x]) {
        return 0;
    }

    if (equal(location, map->end)) {
        return hikeLength;
    }

    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    map->visited[location.y][location.x] = true;

    if (map->terrain[location.y][location.x].type == PATH) {
        for (int i = 0; i < 4; i++) {
            struct Location next = { location.x + xDeltas[i], location.y + yDeltas[i] };
            int length = longestIcyHike(map, next, hikeLength + 1);

            longest = max(longest, length);
        }
    } else {
        struct Location next = location;

        switch (map->terrain[next.y][next.x].type) {
        case SLOPE_UP:
            --next.y;
            break;
        case SLOPE_DOWN:
            ++next.y;
            break;
        case SLOPE_LEFT:
            --next.x;
            break;
        case SLOPE_RIGHT:
            ++next.x;
            break;
        default:
            break;
        }

        longest = longestIcyHike(map, next, hikeLength + 1);
    }

    map->visited[location.y][location.x] = false;

    return longest;
}

void addIntersection(struct Map *map, struct Location previous, struct Location current, int pathLength) {
    for (int i = 0; i < 2; i++) {
        struct Location first = i == 0 ? previous : current;
        struct Location second = i == 0 ? current : previous;

        if (!map->terrain[first.y][first.x].accessibleIntersections) {
            map->terrain[first.y][first.x].accessibleIntersections = (struct Locations *)calloc(1, sizeof(struct Locations));
        }

        second.distance = pathLength;

        addLocation(map->terrain[first.y][first.x].accessibleIntersections, second);
    }
}

void findIntersections(struct Map *map, struct Location previous, struct Location current) {
    int pathLength = equal(current, previous) ? 0 : 1;
    int yDeltas[] = {-1, 0, 1, 0};
    int xDeltas[] = {0, 1, 0, -1};
    struct Locations *steps = (struct Locations *)calloc(1, sizeof(struct Locations));

    while (true) {
        map->terrain[current.y][current.x].type = FOREST;
        ++pathLength;

        if (equal(current, map->end)) {
            addIntersection(map, previous, current, pathLength - 1);
        }

        steps->count = 0;

        for (int i = 0; i < 4; i++) {
            struct Location next = { current.x + xDeltas[i], current.y + yDeltas[i] };

            if (!(next.x >= 0 && next.x < map->width && next.y >= 0 && next.y < map->height)) {
                continue;
            }

            if (equal(next, previous)) {
                continue;
            }

            if (map->terrain[next.y][next.x].type != FOREST) {
                addLocation(steps, next);
            }

            if (map->terrain[next.y][next.x].accessibleIntersections) {
                addIntersection(map, previous, next, pathLength);
            }
        }

        if (steps->count == 1) {
            current = *steps->data;
        } else if (steps->count > 0) {
            addIntersection(map, previous, current, pathLength - 1);

            for (int i = 0; i < steps->count; i++) {
                findIntersections(map, current, steps->data[i]);
            }
        } else {
            break;
        }
    }

    free(steps->data);
    free(steps);
}

int longestDryHike(struct Map *map, struct Location location, struct Location end, int hikeLength) {
    if (equal(location, end)) {
        return hikeLength;
    }

    map->visited[location.y][location.x] = true;

    int longest = 0;

    struct Locations *intersections = map->terrain[location.y][location.x].accessibleIntersections;

    for (int i = 0; i < intersections->count; i++) {
        struct Location intersection = intersections->data[i];

        if (map->visited[intersection.y][intersection.x]) {
            continue;
        }

        int length = longestDryHike(map, intersection, end, hikeLength + intersection.distance);

        longest = max(longest, length);
    }

    map->visited[location.y][location.x] = false;

    return longest;
}

int longestHike(struct Map *map, enum HikeType type) {
    int longest = 0;

    if (type == DRY) {
        findIntersections(map, map->start, map->start);

        struct Location end = *map->terrain[map->end.y][map->end.x].accessibleIntersections->data;

        longest = end.distance + longestDryHike(map, map->start, end, 0);
    } else if (type == ICY) {
        longest = longestIcyHike(map, map->start, 0);
    }

    return longest;    
}

struct Map *getMap(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Map *map = NULL;

    if (inputFile) {
        map = (struct Map *)calloc(1, sizeof(struct Map));
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

        map->terrain = (struct MapTerrain **)malloc(map->height * sizeof(struct MapTerrain *));
        map->visited = (bool **)malloc(map->height * sizeof(bool *));

        for (int y = 0; y < map->height; y++) {
            map->terrain[y] = (struct MapTerrain *)calloc(map->width, sizeof(struct MapTerrain));
            map->visited[y] = (bool *)calloc(map->width, sizeof(bool));

            for (int x = 0; x < map->width; x++) {
                switch (fgetc(inputFile)) {
                case '.':
                    map->terrain[y][x].type = PATH;

                    if (y == 0) {
                        map->start.x = x;
                        map->start.y = y;
                    } else if (y == map->height - 1) {
                        map->end.x = x;
                        map->end.y = y;
                    }
                    break;
                case '#':
                    map->terrain[y][x].type = FOREST;
                    break;
                case '^':
                    map->terrain[y][x].type = SLOPE_UP;
                    break;
                case 'v':
                    map->terrain[y][x].type = SLOPE_DOWN;
                    break;
                case '<':
                    map->terrain[y][x].type = SLOPE_LEFT;
                    break;
                case '>':
                    map->terrain[y][x].type = SLOPE_RIGHT;
                    break;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return map;
}
