/* One positive int of a random bit length (1 to 31 for small cases, always
 * 31 for full-size ones, so bit 30 is set), whose highest bit is 1 and
 * whose other bits follow a pattern the seed picks: random, mostly ones,
 * mostly zeros, all zeros (a power of two) or all ones (2^k - 1). */
void gen(int small)
{
    int width = small ? (int)rnd(1, 31) : 31, shape = (int)rnd(0, 4), i;
    long long n = 1LL << (width - 1);

    for (i = 0; i < width - 1; i++) {
        int on = shape == 0 ? rnd_chance(50)
               : shape == 1 ? rnd_chance(90)
               : shape == 2 ? rnd_chance(10)
               : shape == 4;

        if (on)
            n |= 1LL << i;
    }
    put_int(n);
}
