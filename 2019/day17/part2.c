/* Day 17, part 2 = 1356191 */

#include "../../aoc.h"
#include "scaffold.h"
#include <string.h>
#include <ctype.h>
#include <time.h>

#define VIDEO_FEED 0
#define HALT -1L

void addTurn(char *movements, enum Command command) {
    if (strlen(movements) > 0) {
        strcat(movements, ",");
    }

    sprintf(movements + strlen(movements), "%c", command == TURN_LEFT ? 'L' : 'R');
}

void addSteps(char *movements, int steps) {
    if (steps > 0) {
        if (strlen(movements) > 0) {
            strcat(movements, ",");
        }

        sprintf(movements + strlen(movements), "%d", steps);
    }
}

void msleep(long msec) {
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    while(nanosleep(&ts, &ts));
}

int matches(char *routine, char *movements) {
    int count = 0;
    char *match = strstr(movements, routine);

    while (match) {
        ++count;
        movements += match - movements;

        match = strstr(match + 1, routine);
    }

    return count;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    
    struct Scaffold *scaffold = getScaffold(aoc.input);
    int answer = 0;

    if (scaffold) {
        char movements[256] = "";
        int steps = 0;
        struct Location location;
        bool moving = true;
        enum Command commands[] = { FORWARD, TURN_LEFT, TURN_RIGHT };
        enum Command command;
  
        while (moving) {
            moving = false;

            scaffold->grid[scaffold->robot.location.y][scaffold->robot.location.x] = scaffold->robot.direction;

            for (int i = 0; !moving && i < 3; i++) {
                command = commands[i];

                location = updatedLocation(scaffold->robot, command);

                if (locationValid(scaffold, location)) {
                    if (command == FORWARD) {
                        scaffold->robot.location = location;
                        ++steps;
                    } else {
                        scaffold->robot.direction = updatedDirection(scaffold->robot.direction, command);
                        addSteps(movements, steps);
                        addTurn(movements, command);
                        steps = 0;
                    }

                    moving = true;
                }
            }

            if (!moving) {
                addSteps(movements, steps);
            }
        }

        char main[32] = "";
        char a[21];
        char b[21];
        char c[21];
        char *routines[] = { main, a, b, c };
        char routineNames[] = { 'X', 'A', 'B', 'C' };
        char *routine;
        int routineLength;
        int movementCount;
        char *routineStart = movements;
        char *character;

        for (int i = 1; i < 4; i++) {
            routine = routines[i];
            routineLength = 0;
            movementCount = 0;

            character = routineStart;

            while (*character && movementCount < 4) {
                movementCount += *character == ',' ? 1 : 0;
                ++character;
            }

            routineLength = character - routineStart - 1;

            strncpy(routine, routineStart, routineLength);
            routine[routineLength] = '\0';

            int initialMatches = matches(routine, routineStart + 1);

            character = routineStart + routineLength;

            while (routineLength < 20 && *character && strncmp(routineStart, character, routineLength) != 0) {
                ++character;
            }

            while (routineLength < 20 && *(character + routineLength) && strncmp(routineStart, character, routineLength + 1) == 0) {
                ++routineLength;
            }

            while (*(routineStart + routineLength) != ',' || !isdigit(*(routineStart + routineLength - 1))) {
                --routineLength;
            }

            strncpy(routine, routineStart, routineLength);
            routine[routineLength] = '\0';

            while (matches(routine, routineStart + 1) < initialMatches) {
                --routineLength;

                while (*(routineStart + routineLength) != ',') {
                    --routineLength;
                }

                while (*(routineStart + routineLength) != ',' || !isdigit(*(routineStart + routineLength - 1))) {
                    --routineLength;
                }

                strncpy(routine, routineStart, routineLength);
                routine[routineLength] = '\0';
            }

            if (i < 3) {
                routineStart += routineLength + 1;

                bool newRoutine = false;

                while (!newRoutine) {
                    newRoutine = true;

                    for (int j = 1; newRoutine && j <= i; j++) {
                        if (strncmp(routineStart, routines[j], strlen(routines[j])) == 0) {
                            newRoutine = false;
                            routineStart += strlen(routines[j]) + 1;
                        }
                    }
                }
            }
        }

        routine = movements;

        while (routine && *routine) {
            bool routineFound = false;

            for (int i = 1; i < 4; i++) {
                if (strncmp(routine, routines[i], strlen(routines[i])) == 0) {
                    routineFound = true;

                    sprintf(main + strlen(main), "%s%c", (strlen(main) > 0 ? "," : ""), routineNames[i]);

                    routine += strlen(routines[i]) + 1;
                }
            }

            if (!routineFound) {
                routine = NULL;
            }
        }

        *scaffold->program->data = 2;
        scaffold->program->haltValue = HALT;

        pipe(scaffold->program->input);
        pipe(scaffold->program->output);

        if (fork() == 0) {
            runProgram(scaffold->program);

            exit(0);
        }

        long data;
        int newLines;

        for (int i = 0; i < 4; i++) {
            character = routines[i];

            while (*character) {
                data = *character;
                write(scaffold->program->input[WRITE], &data, sizeof(data));

                ++character;
            }

            data = '\n';
            write(scaffold->program->input[WRITE], &data, sizeof(data));
        }

        data = VIDEO_FEED ? 'y' : 'n';
        write(scaffold->program->input[WRITE], &data, sizeof(data));
        data = '\n';
        write(scaffold->program->input[WRITE], &data, sizeof(data));

        if (VIDEO_FEED) {
            printf("\033[0;0H");
            msleep(20);
        }

        while (read(scaffold->program->output[READ], &data, sizeof(data)) && data != HALT) {
            if (data < 256 && VIDEO_FEED) {
                if (newLines == 2) {
                    msleep(10);
                    printf("\033[0;0H");
                }

                printf("%c", (char)data);

                newLines = newLines + (data == '\n' ? 1 : -newLines);
            }

            answer = data;
        }

        freeScaffold(scaffold);
    }

    print(aoc, INT, &answer);

    return 0;
}

// L,12,L,8,R,12,L,10,L,8,L,12,R,12,L,12,L,8,R,12,R,12,L,8,L,10,L,12,L,8,R,12,L,12,L,8,R,12,R,12,L,8,L,10,L,10,L,8,L,12,R,12,R,12,L,8,L,10,L,10,L,8,L,12,R,12

// Main: A,B,A,C,A,A,C,B,C,B
// A: L,12,L,8,R,12
// B: L,10,L,8,L,12,R,12
// C: R,12,L,8,L,10

// ..............v<<<<<<<<<<................................
// ..............v.........^................................
// ..............v.........^................................
// ..............v.........^................................
// ..............v.........^................................
// ..............v.........^................................
// ..............v.........^................................
// ..............v.........^................................
// ............>>v>>>>>>>>>^................................
// ............^.v..........................................
// ............^.v..........................................
// ............^.v..........................................
// ............^.>>>>>>>>v..................................
// ............^.........v..................................
// ............^.........v..................................
// ............^.........v..................................
// ............^.........v...........>>>>>>>>>>>>v..........
// ............^.........v...........^...........v..........
// ............^.........v...........^.........v<<<<<<<<<<..
// ............^.........v...........^.........v.v.......^..
// ............^<<<<<<<<.v...........^.........v.v.......^..
// ....................^.v...........^.........v.v.......^..
// ....................^.v...........^.........v.v.......^..
// ....................^.v...........^.........v.v.......^..
// ....................^.>>>>>>>>>>>>^.........v.v.......^..
// ....................^.......................v.v.......^..
// ............v<<<<<<<^<<<<...................>>>>>>>>>>>>v
// ............v.......^.........................v.......^.v
// ............v.......^.........................>>>>>>>>^.v
// ............v.......^...................................v
// ............v.......^...................................v
// ............v.......^...................................v
// ........>>>>>>>>>>>>^...................................v
// ........^...v...........................................v
// v<<<<<<<^<<<<...........................................v
// v.......^...............................................v
// v.......^.....................................v<<<<<<<<.v
// v.......^.....................................v.......^.v
// v.......^...................................v<v<<<<<<<^<<
// v.......^...................................v.v.......^..
// v.......^...................................v.v.......^..
// v.......^...................................v.v.......^..
// v.......^...................................v.v.......^..
// v.......^...................................v.v.......^..
// >>>>>>>>^...................................v.v.......^..
// ............................................v.v.......^..
// ............................................>>v>>>>>>>^..
// ..............................................v..........
// ..................................<<<<<<<<<<<<<..........