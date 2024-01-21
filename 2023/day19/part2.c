/* Day 19, part 2 = 121964982771486 */

#include "../../aoc.h"
#include "parts.h"

struct PartCategoryRange {
    char name[8];

    long xMin, xMax;
    long mMin, mMax;
    long aMin, aMax;
    long sMin, sMax;
};

void getPartCategoryRange(struct PartCategoryRange range, char category, long *min, long *max) {
    switch (category) {
    case 'x':
        *min = range.xMin;
        *max = range.xMax;
        break;
    case 'm':
        *min = range.mMin;
        *max = range.mMax;
        break;
    case 'a':
        *min = range.aMin;
        *max = range.aMax;
        break;
    case 's':
        *min = range.sMin;
        *max = range.sMax;
        break;
    }
}

void setPartCategoryRange(struct PartCategoryRange *range, char category, long min, long max) {
    switch (category) {
    case 'x':
        range->xMin = min;
        range->xMax = max;
        break;
    case 'm':
        range->mMin = min;
        range->mMax = max;
        break;
    case 'a':
        range->aMin = min;
        range->aMax = max;
        break;
    case 's':
        range->sMin = min;
        range->sMax = max;
        break;
    }
}

struct PartCategoryRanges {
    int capacity;
    int count;
    struct PartCategoryRange *data;
};

void addRange(struct PartCategoryRanges *ranges, struct PartCategoryRange range) {
    if (ranges->count == ranges->capacity) {
        ranges->capacity += 5;
        ranges->data = (struct PartCategoryRange *)realloc(ranges->data, ranges->capacity * sizeof(struct PartCategoryRange));
    }

    ranges->data[ranges->count++] = range;
}

struct PartCategoryRange popRange(struct PartCategoryRanges *ranges) {
    return ranges->data[--ranges->count];
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct SortingData *data = getSortingData(aoc.input);
    long answer = 0;

    if (data) {
        struct PartCategoryRanges *queue = (struct PartCategoryRanges *)calloc(1, sizeof(struct PartCategoryRanges));
        struct PartCategoryRanges *accepted = (struct PartCategoryRanges *)calloc(1, sizeof(struct PartCategoryRanges));

        addRange(queue, (struct PartCategoryRange){ "in", 1, 4000, 1, 4000, 1, 4000, 1, 4000 });

        while (queue->count) {
            struct PartCategoryRange range = popRange(queue);
            struct Workflow *workflow = findWorkflow(data->workflows, range.name);

            if (workflow) {
                for (int i = 0; i < workflow->rules->count; i++) {
                    struct Rule rule = workflow->rules->data[i];

                    if (rule.category) {
                        struct PartCategoryRange newRange = range;
                        long min, max;

                        strcpy(newRange.name, rule.result);

                        getPartCategoryRange(range, rule.category, &min, &max);

                        if (rule.condition == GREATER_THAN) {
                            setPartCategoryRange(&newRange, rule.category, rule.value + 1, max);
                            setPartCategoryRange(&range, rule.category, min, rule.value);
                        } else if (rule.condition == LESS_THAN) {
                            setPartCategoryRange(&newRange, rule.category, min, rule.value - 1);
                            setPartCategoryRange(&range, rule.category, rule.value, max);
                        }

                        addRange(*newRange.name == 'A' ? accepted : queue, newRange);
                    } else {
                        struct PartCategoryRange newRange = range;

                        strcpy(newRange.name, rule.result);

                        addRange(*newRange.name == 'A' ? accepted : queue, newRange);
                    }
                }
            }
        }

        for (int i = 0; i < accepted->count; i++) {
            struct PartCategoryRange range = accepted->data[i];

            answer += (range.xMax - range.xMin + 1) *
                      (range.mMax - range.mMin + 1) *
                      (range.aMax - range.aMin + 1) *
                      (range.sMax - range.sMin + 1);
        }

        free(queue->data);
        free(queue);

        free(accepted->data);
        free(accepted);

        freeSortingData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
