/* Day 10, part 2 = dc7e7dee710d4c7201ce42713e6b8359 */

#include "../../aoc.h"
#include "hash.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    char answer[33] = "";

    if (inputFile) {
        struct Numbers *numbers = getNumbers();
        char c;
        int hash;
        char *position;

        for (int i = 0; i < 64; i++) {
            fseek(inputFile, 0, SEEK_SET);

            while (!feof(inputFile) && (c = fgetc(inputFile)) > 0) {
                knot(numbers, (int)c);
            }

            knot(numbers, 17);
            knot(numbers, 31);
            knot(numbers, 73);
            knot(numbers, 47);
            knot(numbers, 23);
        }

        fclose(inputFile);

        position = answer;

        for (int i = 0; i < 16; i++) {
            hash = 0;

            for (int j = 0; j < 16; j++) {
                hash ^= numbers->list[i * 16 + j];
            }

            sprintf(position, "%02x", hash);
            position += 2;
        }

        free(numbers);
    }

    print(aoc, STRING, answer);

    return 0;
}
