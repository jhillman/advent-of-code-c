#include <stdlib.h>

struct PhaseSequence {
    int a;
    int b;
    int c;
    int d;
    int e;
};

struct PhaseSequenceData {
    int phaseSequenceLength;
    int phaseSequenceCount;
    struct PhaseSequence *phaseSequences;
};

void fillPhaseSequences(struct PhaseSequence *phaseSequences, int *phaseSequenceIndex, int number, int minDigit, int maxDigit, int remainingDigitCount, int usedDigits[]) {
    if (remainingDigitCount == 0) {
        int phaseSequenceLength = sizeof(struct PhaseSequence) / sizeof(int);
        struct PhaseSequence phaseSequence;
        
        for (int i = 0; i < phaseSequenceLength; i++) {
            *((int *)&phaseSequence + i) = number % 10 + minDigit;
            number /= 10;
        }

        phaseSequences[*phaseSequenceIndex] = phaseSequence;

        ++(*phaseSequenceIndex);
    } else {
        for (int i = 0; i <= maxDigit - minDigit; i++) {
            if (!usedDigits[i]) {
                usedDigits[i] = 1;
                fillPhaseSequences(phaseSequences, phaseSequenceIndex, number * 10 + i, minDigit, maxDigit, remainingDigitCount - 1, usedDigits);
                usedDigits[i] = 0;
            }
        }
    }
}

struct PhaseSequenceData getPhaseSequenceData(int minDigit, int maxDigit) {
    struct PhaseSequenceData data;

    data.phaseSequenceLength = sizeof(struct PhaseSequence) / sizeof(int);
    data.phaseSequenceCount = 1;
    struct PhaseSequence phaseSequence;

    for (int i = 0; i < data.phaseSequenceLength; i++) {
        *((int *)&phaseSequence + i) = i;
        data.phaseSequenceCount *= (data.phaseSequenceLength - i);
    }

    data.phaseSequences = (struct PhaseSequence *)calloc(data.phaseSequenceCount, sizeof(struct PhaseSequence));
    int usedDigits[10] = { 0 };
    int phaseSequenceIndex = 0;

    fillPhaseSequences(data.phaseSequences, &phaseSequenceIndex, 0, minDigit, maxDigit, data.phaseSequenceLength, usedDigits);

    return data;
}