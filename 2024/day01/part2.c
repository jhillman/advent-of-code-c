/* Day 1, part 2 = 27647262 */

#include "../../aoc.h"
#include "lists.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int *listOne;
    int *listTwo;
    int numberCount;
    int answer = 0;

    getLists(aoc.input, &listOne, &listTwo, &numberCount);

    if (listOne && listTwo) {
        int number;
        int score = 0;

        for (int i = 0; i < numberCount; i++) {
            number = listOne[i];

            for (int j = 0; j < numberCount; j++) {
                if (listTwo[j] == number) {
                    score += number;
                }
            }
        }

        answer = score;

        free(listOne);
        free(listTwo);
    }

    print(aoc, INT, &answer);

    return 0;
}
