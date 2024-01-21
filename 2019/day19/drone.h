enum Coordinate {
    X,
    Y
};

struct Drone {
    enum Coordinate coordinate;
    int x;
    int y;
    bool pulled;
};

long input(void *context) {
    struct Drone *drone = (struct Drone *)context;

    long input = drone->coordinate == X ? drone->x : drone->y;
    drone->coordinate = !drone->coordinate;

    return input;
}

void output(void *context, long output) {
    struct Drone *drone = (struct Drone *)context;

    drone->pulled = output == 1;
}

bool dronePulled(struct Program *program, struct Drone *drone, int x, int y) {
    if (!program->io) {
        setIO(program, &input, &output, drone);
    }

    drone->x = x;
    drone->y = y;

    runProgram(program);
    resetProgram(program);

    return drone->pulled;    
}