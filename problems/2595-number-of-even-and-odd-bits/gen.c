/* One n from 1 to 1000, in a shape the seed picks: any value (from 512 up
 * for full-size cases, so bit 9 is set); a power of two; 2^k - 1; only
 * even or only odd positions set; or a value near 1000, where most of the
 * ten bits are 1. */
void gen(int small)
{
    int shape = (int)rnd(0, 4), n = 0, k, i, parity;

    if (shape == 0) {
        n = (int)rnd(small ? 1 : 512, 1000);
    } else if (shape == 1) {
        k = (int)rnd(0, 9);
        n = 1 << k;
    } else if (shape == 2) {
        k = (int)rnd(1, 9);
        n = (1 << k) - 1;
    } else if (shape == 3) {
        parity = (int)rnd(0, 1);
        for (i = parity; i < 10; i += 2)
            if (rnd_chance(60))
                n |= 1 << i;
        if (n == 0)
            n = 1 << parity;
    } else {
        n = 1000 - (int)rnd(0, 60);
    }
    put_int(n);
}
