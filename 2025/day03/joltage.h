#include <stdlib.h>

long totalJoltage(char *input, int batteryCount) {
    FILE *inputFile = fopen(input, "r");
    long total = 0;

    if (inputFile) {
        int bankSize = 0;
        char *bank;

        do {
            ++bankSize;
        } while (fgetc(inputFile) != '\n');

        bank = (char *)malloc(bankSize);

        fseek(inputFile, 0, SEEK_SET);

        while (fgets(bank, bankSize + 1, inputFile)) {
            bank[strcspn(bank, "\n")] = 0;

            long joltage = 0;
            char *maxBattery = bank;

            for (int i = batteryCount; i > 0; i--) {
                char *battery = maxBattery + 1;

                while (*battery && battery - bank < bankSize - i) {
                    if (*battery > *maxBattery) {
                        maxBattery = battery;
                    }

                    ++battery;
                }

                joltage *= 10;
                joltage += *maxBattery - '0';

                ++maxBattery;
            }

            total += joltage;
        }

        free(bank);

        fclose(inputFile);
    }

    return total;
}
