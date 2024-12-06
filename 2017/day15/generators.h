struct Generator {
    long factor;
    long value;
};

void generate(struct Generator *generator, long mask) {
    do {
        generator->value *= generator->factor;
        generator->value = (generator->value & 2147483647) + (generator->value >> 31);
        generator->value = generator->value >> 31 ? generator->value - 2147483647 : generator->value;
    } while ((generator->value & mask) != 0);
}

int matchingPairs(char *input, long maskA, long maskB, int pairs) {
    FILE *inputFile = fopen(input, "r");
    int matchingPairs = 0;

    if (inputFile) {
        struct Generator a = { 16807 };
        struct Generator b = { 48271 };

        fscanf(inputFile, "Generator A starts with %ld\n", &a.value);
        fscanf(inputFile, "Generator B starts with %ld", &b.value);
        fclose(inputFile);

        for (int i = 0; i < pairs; i++) {
            generate(&a, maskA);
            generate(&b, maskB);

            if ((a.value & 0xffff) == (b.value & 0xffff)) {
                ++matchingPairs;
            }
        }
    }

    return matchingPairs;
}
