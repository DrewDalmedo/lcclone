/* A program of bits ending in 0: 1 to 10 bits for small cases, the
 * largest length, 1000, for full-size ones. The seed picks how the bits
 * before the final 0 are filled: at random, mostly with 1s (long runs), or
 * at random and then a run of 1s of random length, odd or even and up to
 * the whole program, right before the final 0. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 1000;
    int shape = (int)rnd(0, 2), run, i;
    int *bits = malloc((size_t)n * sizeof *bits);

    for (i = 0; i < n - 1; i++)
        bits[i] = rnd_chance(shape == 1 ? 85 : 50);
    if (shape == 2) {
        run = (int)rnd(0, n - 1);
        for (i = n - 1 - run; i < n - 1; i++)
            bits[i] = 1;
        if (n - 2 - run >= 0)
            bits[n - 2 - run] = 0;
    }
    bits[n - 1] = 0;
    put_ints(bits, n);
    free(bits);
}
