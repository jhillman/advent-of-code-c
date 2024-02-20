#include "../../aoc.h"
#include <stdlib.h>

struct DateTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
};

struct Guard {
    int id;
    int number;
    int sleepPerMinute[60];
    int totalSleep;
};

struct Guards {
    int capacity;
    int count;
    struct Guard *data;
};

int getGuardId(struct Guards *guards, int number) {
    int id = -1;

    for (int i = 0; id == -1 && i < guards->count; i++) {
        if (guards->data[i].number == number) {
            id = guards->data[i].id;
        }
    }

    if (id == -1) {
        struct Guard guard = { guards->count, number, { 0 }, 0 };

        guards->data = (struct Guard *)realloc(guards->data, (guards->count + 1) * sizeof(struct Guard));
        guards->data[guards->count++] = guard;

        id = guard.id;
    }

    return id;
}

struct Event {
    struct DateTime dateTime;
    char description[32];
};

struct Events {
    int capacity;
    int count;
    struct Event *data;
};

int compareEvents(const void *a, const void *b) {
    struct Event *eventA = (struct Event *)a;
    struct Event *eventB = (struct Event *)b;

    int result = eventA->dateTime.year - eventB->dateTime.year;

    if (result == 0) {
        result = eventA->dateTime.month - eventB->dateTime.month;
    }

    if (result == 0) {
        result = eventA->dateTime.day - eventB->dateTime.day;
    }

    if (result == 0) {
        result = eventA->dateTime.hour - eventB->dateTime.hour;
    }

    if (result == 0) {
        result = eventA->dateTime.minute - eventB->dateTime.minute;
    }

    return result;
}

void addEvent(struct Events *events, struct Event event) {
    if (events->count == events->capacity) {
        events->capacity += 100;
        events->data = (struct Event *)realloc(events->data, events->capacity * sizeof(struct Event));
    }

    events->data[events->count++] = event;
}

struct SleepData {
    struct Events *events;
    struct Guards *guards;
    int sleepiestGuardMinute;
    int consistentGuardMinute;
};

void freeSleepData(struct SleepData *data) {
    free(data->events->data);
    free(data->events);
    free(data->guards->data);
    free(data->guards);

    free(data);
}

struct SleepData *getSleepData(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct SleepData *data = NULL;

    if (inputFile) {
        data = (struct SleepData *)calloc(1, sizeof(struct SleepData));
        data->events = (struct Events *)calloc(1, sizeof(struct Events));
        data->guards = (struct Guards *)calloc(1, sizeof(struct Guards));

        struct Event event;
        int guardNumber;
        int guardId;
        int sleepStartMinute;
        int sleepEndMinute;
        int maxTotalSleep = 0;
        int maxMinuteSleep = 0;
        int sleepiestGuardMaxMinuteSleep = 0;

        int sleepiestGuardNumber;
        int sleepiestMinute;

        int consistentGuardNumber;
        int consistentMinute;

        while (fscanf(inputFile, "[%d-%d-%d %d:%d] %[^\n]\n", &event.dateTime.year, &event.dateTime.month, &event.dateTime.day, &event.dateTime.hour, &event.dateTime.minute, event.description) == 6) {
            addEvent(data->events, event);
        }

        fclose(inputFile);

        qsort(data->events->data, data->events->count, sizeof(struct Event), compareEvents);

        for (int i = 0; i < data->events->count; i++) {
            if (sscanf(data->events->data[i].description, "Guard #%d", &guardNumber)) {
                guardId = getGuardId(data->guards, guardNumber);
            } else if (strstr(data->events->data[i].description, "falls asleep")) {
                sleepStartMinute = data->events->data[i].dateTime.minute;
            } else if (strstr(data->events->data[i].description, "wakes up")) {
                sleepEndMinute = data->events->data[i].dateTime.minute;

                data->guards->data[guardId].totalSleep += (sleepEndMinute - sleepStartMinute);

                if (data->guards->data[guardId].totalSleep > maxTotalSleep) {
                    maxTotalSleep = data->guards->data[guardId].totalSleep;

                    if (sleepiestGuardNumber != guardNumber) {
                        sleepiestGuardMaxMinuteSleep = 0;
                    }

                    sleepiestGuardNumber = guardNumber;
                }

                for (int minute = 0; minute < 60; minute++) {
                    if (minute >= sleepStartMinute && minute <= sleepEndMinute) {
                        ++data->guards->data[guardId].sleepPerMinute[minute];
                    }

                    if (sleepiestGuardNumber == guardNumber && data->guards->data[guardId].sleepPerMinute[minute] > sleepiestGuardMaxMinuteSleep) {
                        sleepiestGuardMaxMinuteSleep = data->guards->data[guardId].sleepPerMinute[minute];
                        sleepiestMinute = minute;
                    }

                    if (data->guards->data[guardId].sleepPerMinute[minute] > maxMinuteSleep) {
                        maxMinuteSleep = data->guards->data[guardId].sleepPerMinute[minute];
                        consistentGuardNumber = guardNumber;
                        consistentMinute = minute;
                    }
                }
            }
        }

        data->sleepiestGuardMinute = sleepiestGuardNumber * sleepiestMinute;
        data->consistentGuardMinute = consistentGuardNumber * consistentMinute;
    }

    return data;
}