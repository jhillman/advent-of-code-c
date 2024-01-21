/* Day 7, part 2 = 89603079 */

#include "../../aoc.h"
#include "phase.h"
#include "../intcode.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    long answer = 0;

    struct Program *programA = copyProgram(program, 'a');
    struct Program *programB = copyProgram(program, 'b');
    struct Program *programC = copyProgram(program, 'c');
    struct Program *programD = copyProgram(program, 'e');
    struct Program *programE = copyProgram(program, 'e');

    struct Program *programs[] = {
        programA,
        programB,
        programC,
        programD,
        programE
    };

    struct PhaseSequenceData data = getPhaseSequenceData(5, 9);

    for (int i = 0; i < data.phaseSequenceLength; i++) {
        if (i > 0) {
            programs[i]->input[WRITE] = programs[i - 1]->output[WRITE];
            programs[i]->input[READ] = programs[i - 1]->output[READ];
        }

        pipe(programs[i]->output);
    }

    programA->input[WRITE] = programE->output[WRITE];
    programA->input[READ] = programE->output[READ];

    long maxSignal = 0;

    for (int i = 0; i < data.phaseSequenceCount; i++) {
        struct PhaseSequence phaseSequence = data.phaseSequences[i];

        for (int j = 0; j < data.phaseSequenceLength; j++) {
            long phase = *((int *)&phaseSequence + j);

            write(programs[j]->input[WRITE], &phase, sizeof(phase));
        }

        long aInput = 0;
        write(programA->input[WRITE], &aInput, sizeof(aInput));

        for (int j = 0; j < data.phaseSequenceLength; j++) {
            if (fork() == 0) {
                runProgram(programs[j]);
                resetProgram(programs[j]);

                exit(0);
            }
        }
    
        for (int j = 0; j < data.phaseSequenceLength; j++) {
            wait(NULL); 
        }

        long signal;
        read(programE->output[READ], &signal, sizeof(signal));

        if (signal > maxSignal) {
            maxSignal = signal;
        }
    }

    answer = maxSignal;

    freeProgram(programA);
    freeProgram(programB);
    freeProgram(programC);
    freeProgram(programD);
    freeProgram(programE);

    freeProgram(program);

    free(data.phaseSequences);

    print(aoc, LONG, &answer);

    return 0;
}
