/* Day 4, part 1 = 459 */

#include "../../aoc.h"
#include "sectionrange.h"

bool testSectionRanges(struct SectionRange sectionRange1, struct SectionRange sectionRange2) {
    return sectionRange1.min >= sectionRange2.min && sectionRange1.max <= sectionRange2.max ||
           sectionRange2.min >= sectionRange1.min && sectionRange2.max <= sectionRange1.max;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = checkSectionAssignments(aoc.input);

    print(aoc, INT, &answer);

    return 0;
}
