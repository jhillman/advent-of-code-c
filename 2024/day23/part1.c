/* Day 23, part 1 = 1423 */

#include "../../aoc.h"
#include "computers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Computers *computers = getComputers(aoc.input);
    int answer = 0;

    if (computers) {
        struct Computer *computer1;
        struct Computer *computer2;
        struct Computer *computer3;
        struct Strings *computerNames = (struct Strings *)calloc(1, sizeof(struct Strings));
        struct Strings *computerGroups = (struct Strings *)calloc(1, sizeof(struct Strings));
        char computerGroup[8];

        for (int i = 0; i < computers->count; i++) {
            computer1 = computers->data[i];

            for (int j = 0; j < computer1->connections->count; j++) {
                computer2 = computer1->connections->data[j];

                for (int k = 0; k < computer2->connections->count; k++) {
                    computer3 = computer2->connections->data[k];

                    if (findComputer(computer1->connections, computer3->name, 0, computer1->connections->count) &&
                        (*computer1->name == 't' || *computer2->name == 't' || *computer3->name == 't')) {
                        addString(computerNames, computer1->name, false, false);
                        addString(computerNames, computer2->name, false, false);
                        addString(computerNames, computer3->name, false, true);

                        sprintf(computerGroup, "%s%s%s", 
                            computerNames->data[0]->value, 
                            computerNames->data[1]->value, 
                            computerNames->data[2]->value);
                        computerNames->count = 0;

                        addString(computerGroups, computerGroup, true, true);
                    }
                }
            }
        }

        answer = computerGroups->count;

        freeComputers(computers, true);
        freeStrings(computerNames, false);
        freeStrings(computerGroups, true);
    }

    print(aoc, INT, &answer);

    return 0;
}
