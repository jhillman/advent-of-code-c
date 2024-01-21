/* Day 11, part 2 = 2059 */

#include "../../aoc.h"
#include <stdbool.h>
#include "seats.h"

struct SeatTraverser {
    int rowDelta;
    int columnDelta;
};

int occupiedSeatCount(char **seatLayout, int rowCount, int columnCount, int row, int column) {
    struct SeatTraverser seatTraversers[] = {
        { -1, 0 },
        { 1, 0 },
        { 0, -1 },
        { 0, 1 },
        { -1, -1 },
        { 1, -1 },
        { -1, 1 },
        { 1, 1 }
    };

    int occupiedSeatCount = 0;

    for (int i = 0; i < 8; i++) {
        struct SeatTraverser seatTraverser = seatTraversers[i];

        int rowIndex = row + seatTraverser.rowDelta;
        int columnIndex = column + seatTraverser.columnDelta;
        bool keepLooking = true;

        while (keepLooking && rowIndex >= 0 && rowIndex < rowCount && columnIndex >= 0 && columnIndex < columnCount) {
            switch (seatLayout[rowIndex][columnIndex]) {
                case '#':
                    ++occupiedSeatCount;
                    keepLooking = false;
                    break;
                case '.':
                    rowIndex += seatTraverser.rowDelta;
                    columnIndex += seatTraverser.columnDelta;
                    break;
                case 'L':
                    keepLooking = false;
                    break;
            }
        }
    }

    return occupiedSeatCount;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int rowCount;
    int columnCount;
    char **seatLayout = getSeatLayout(aoc.input, &rowCount, &columnCount);
    int answer = 0;

    if (seatLayout) {
        while (applyRules(seatLayout, rowCount, columnCount, 5));

        int count = totalOccupiedSeatCount(seatLayout, rowCount, columnCount);

        freeLayout(seatLayout, rowCount);

        answer = count;
    }

    print(aoc, INT, &answer);

    return 0;
}
