/* Day 22, part 2 = 93750418158025 */

#include "../../aoc.h"
#include <string.h>

struct ModInt {
    long value;
    long modulus;
};

struct ModInt add(struct ModInt left, struct ModInt right) {
    left.value = (left.value + right.value) % left.modulus;

    return left;
}

struct ModInt subtract(struct ModInt left, struct ModInt right) {
    struct ModInt value = { left.modulus - right.value,  left.modulus };

    return add(left, value);
}

struct ModInt multiply(struct ModInt left, struct ModInt right) {
    struct ModInt multiplier = { left.value, left.modulus };
    long multiplicand = right.value;

    left.value = 0;

    while (multiplicand > 0) {
        if (multiplicand & 1) {
            left = add(left, multiplier);
        }

        multiplicand = multiplicand >> 1;
        multiplier = (struct ModInt){ (multiplier.value * 2) % multiplier.modulus, multiplier.modulus };
    }

    return left;
}

struct ModInt power(struct ModInt base, long exponent) {
    struct ModInt result = { 1, base.modulus };

    while (exponent > 0) {
        if (exponent & 1) {
            result = multiply(result, base);
        }

        exponent = exponent >> 1;
        base = multiply(base, base);
    }

    return result;
}

#define CARD_COUNT 10007

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    long modulus = 119315717514047;
    long shuffles = 101741582076661;

    char line[32];
    int value;
    struct ModInt a = { 1, modulus };
    struct ModInt b = { 0, modulus };

    long answer = 0;

    if (inputFile) {
        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            long av = 1;
            long bv = 0;

            if (strcmp(line, "deal into new stack") == 0) {
                av = modulus - 1;
                bv = modulus - 1;
            } else if (sscanf(line, "cut %d", &value)) {
                av = 1;
                bv = (modulus - value) % modulus;
            } else if (sscanf(line, "deal with increment %d", &value)) {
                av = value;
                bv = 0;
            }

            struct ModInt am = { av, modulus };
            struct ModInt bm = { bv, modulus };

            a = multiply(a, am);

            b = multiply(b, am);
            b = add(b, bm);
        }

        struct ModInt offset = { 2020, modulus };
        struct ModInt ai = power(a, modulus - 2);
        struct ModInt bi = { modulus - b.value, modulus };
        struct ModInt one = { 1, modulus };

        struct ModInt value = add(multiply(offset, power(ai, shuffles)), multiply(ai, multiply(bi, multiply(subtract(power(ai, shuffles), one), power(subtract(ai, one), modulus - 2)))));

        answer = value.value;

        fclose(inputFile);
    }

    print(aoc, LONG, &answer);

    return 0;
}