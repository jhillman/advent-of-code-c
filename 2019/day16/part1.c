/* Day 16, part 1 = 77038830 */

#include "../../aoc.h"
#include "numbers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct NumberList *list = getNumberList(aoc.input);
    int answer = 0;

    if (list) {
        struct NumberList *output = copyList(list);
        struct NumberList *swap;
        int pattern[] = { 0, 1, 0, -1 };
        int phases = 100;

        while (phases--) {
            for (int i = 0; i < list->length; i++) {
                int sum = 0;

                for (int j = 0; j < list->length; j++) {
                    sum += pattern[((j + 1) / (i + 1)) % 4] * list->numbers[j];
                }

                output->numbers[i] = abs(sum) % 10;
            }

            if (phases > 0) {
                swap = list;
                list = output;
                output = swap;
            }
        }

        for (int i = 0; i < 8; i++) {
            answer = (answer * 10) + output->numbers[i] % 10;
        }

        freeNumberList(list);
        freeNumberList(output);
    }

    
    print(aoc, INT, &answer);

    return 0;
}
