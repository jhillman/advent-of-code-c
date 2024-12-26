/* Day 22, part 2 = 1938 */

#include "../../aoc.h"
#include "secret.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Secrets *secrets = getSecrets(aoc.input);
    long answer = 0;

    if (secrets) {
        struct SequencePrice ****sequencePrices = getSequencePrices();
        struct SequencePrice *sequencePrice;
        int maximumPrice = 0;

        struct Secret secret = {
            0, 0, 0,
            (struct ChangeSequence *)calloc(1, sizeof(struct ChangeSequence))
        };

        for (int i = 0; i < secrets->count; i++) {
            initialize(&secret, secrets->data[i]);
            reset(sequencePrices);

            for (int i = 0; i < 2000; i++) {
                next(&secret);

                if (secret.sequence->ready) {
                    update(secret.sequence);

                    sequencePrice = &sequencePrices[secret.sequence->change1]
                                                   [secret.sequence->change2]
                                                   [secret.sequence->change3]
                                                   [secret.sequence->change4];

                    if (!sequencePrice->set) {
                        sequencePrice->set = true;
                        sequencePrice->total += secret.price;

                        if (sequencePrice->total > maximumPrice) {
                            maximumPrice = sequencePrice->total;
                        }
                    }
                }
            }
        }

        freeSecrets(secrets);
        freeSequencePrices(sequencePrices);
        free(secret.sequence);

        answer = maximumPrice;
    }

    print(aoc, LONG, &answer);

    return 0;
}