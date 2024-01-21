#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int markerPosition(char *input) {
	FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char character;
        char characters[MARKER_LENGTH + 1];
        char temp[MARKER_LENGTH];
        int position = 0;

        characters[MARKER_LENGTH] = 0;

        while (fscanf(inputFile, "%c", &character)) {
            if (position < MARKER_LENGTH) {
                characters[position++] = character;
            } else {
                strcpy(temp, characters + 1);
                strcpy(characters, temp);
                characters[MARKER_LENGTH - 1] = character;

                ++position;

                bool allUnique = true;

                for (int i = 0; allUnique && i < MARKER_LENGTH; i++) {
                    if (strchr(characters, characters[i]) != &characters[i]) {
                        allUnique = false;
                    }
                }

                if (allUnique) {
                    break;
                }
            }
        }

        fclose(inputFile);

        return position;
    }

    return 0;
}