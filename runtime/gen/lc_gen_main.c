/*
 * lc_gen_main.c: the main() of a pack's generator, built after gen.c.
 *
 *     gen SEED small|large [COUNT]
 *
 * prints COUNT cases (default 1), for the seeds SEED, SEED+1, ..., with a
 * blank line between them. Each case is made by gen() after the random
 * numbers start afresh from its own seed, so a case is the same whether
 * it is made alone or in a batch.
 */

static unsigned long long lc_gen_number(const char *s)
{
    unsigned long long v = 0;
    const char *p = s;

    if (!*p)
        lc_gen_fail("expected a number");
    for (; *p; p++) {
        unsigned d = (unsigned)(*p - '0');
        if (d > 9 || v > (18446744073709551615ull - d) / 10)
            lc_gen_fail("expected a number");
        v = v * 10 + d;
    }
    return v;
}

int main(int argc, char **argv)
{
    unsigned long long seed, count = 1, i;
    int small;

    if (argc < 3 || argc > 4 ||
        (strcmp(argv[2], "small") != 0 && strcmp(argv[2], "large") != 0)) {
        fputs("usage: gen SEED small|large [COUNT]\n", stderr);
        return 2;
    }
    seed = lc_gen_number(argv[1]);
    small = strcmp(argv[2], "small") == 0;
    if (argc == 4)
        count = lc_gen_number(argv[3]);
    for (i = 0; i < count; i++) {
        if (i)
            putchar('\n');
        lc_gen_start(seed + i);
        gen(small);
    }
    if (fflush(stdout) != 0 || ferror(stdout)) {
        perror("gen: writing the cases");
        return 1;
    }
    return 0;
}
