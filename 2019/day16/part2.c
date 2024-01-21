/* Day 16, part 2 = 28135104 */

#include "../../aoc.h"
#include "numbers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct NumberList *list = getNumberList(aoc.input);
    int answer = 0;

    if (list) {
        int offset = 0;
        int phases = 100;

        for (int i = 0; i < 7; i++) {
            offset = (offset * 10) + list->numbers[i] % 10;
        }

        struct NumberList *message = (struct NumberList *)calloc(1, sizeof(struct NumberList));
        message->length = list->length * 10000 - offset;
        message->numbers = (int *)malloc(message->length * sizeof(int));

        for (int i = 0; i < message->length; i++) {
            message->numbers[i] = list->numbers[(offset + i) % list->length];
        }

        freeNumberList(list);

        while (phases--) {
            int sum = 0;

            for (int i = message->length - 1; i >= 0; i--) {
                sum += message->numbers[i];
                message->numbers[i] = abs(sum) % 10;
            }
        }

        for (int i = 0; i < 8; i++) {
            answer = (answer * 10) + message->numbers[i] % 10;
        }

        freeNumberList(message);
    }

    
    print(aoc, INT, &answer);

    return 0;
}
