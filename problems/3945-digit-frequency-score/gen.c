/* Small cases have 1 to 5 digits (now and then 9, or n = 10^9), drawn from
 * a pool of one to three digits so that repeats are common. Full-size
 * ones, by the seed: n = 10^9; nine digits that are mostly 9s; any n from
 * 1 to 10^9; or nine digits with zeros between non-zero ends. */
static long long from_pool(int len, const int *pool, int kinds)
{
    long long n = 0;
    int i, d;

    for (i = 0; i < len; i++) {
        d = pool[rnd(0, kinds - 1)];
        if (i == 0 && d == 0)
            d = (int)rnd(1, 9);
        n = n * 10 + d;
    }
    return n;
}

void gen(int small)
{
    long long n = 0;
    int pool[3], kinds, len, shape, i, d;

    if (small) {
        if (rnd_chance(8)) {
            put_int(1000000000);
            return;
        }
        len = rnd_chance(20) ? 9 : (int)rnd(1, 5);
        kinds = (int)rnd(1, 3);
        for (i = 0; i < kinds; i++)
            pool[i] = (int)rnd(0, 9);
        n = from_pool(len, pool, kinds);
    } else {
        shape = (int)(gen_seed() % 4);
        if (shape == 0) {
            n = 1000000000;
        } else if (shape == 1) {
            for (i = 0; i < 9; i++) {
                d = rnd_chance(80) ? 9 : (int)rnd(0, 9);
                if (i == 0 && d == 0)
                    d = 9;
                n = n * 10 + d;
            }
        } else if (shape == 2) {
            n = rnd(1, 1000000000);
        } else {
            for (i = 0; i < 9; i++) {
                if (i == 0 || i == 8)
                    d = (int)rnd(1, 9);
                else
                    d = rnd_chance(75) ? 0 : (int)rnd(1, 9);
                n = n * 10 + d;
            }
        }
    }
    put_int(n);
}
