/* Day 22, part 1 = 17163502021 */

#include "../../aoc.h"
#include "secret.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Secrets *secrets = getSecrets(aoc.input);
    long answer = 0;

    if (secrets) {
        struct Secret secret = { 0, 0, 0, NULL };
        long sum = 0;

        for (int i = 0; i < secrets->count; i++) {
            initialize(&secret, secrets->data[i]);

            for (int j = 0; j < 2000; j++) {
                next(&secret);
            }

            sum += secret.value;
        }

        freeSecrets(secrets);

        answer = sum;
    }

    print(aoc, LONG, &answer);

    return 0;
}
