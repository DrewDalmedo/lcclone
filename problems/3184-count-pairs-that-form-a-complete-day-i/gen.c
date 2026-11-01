/* Each length is 24 * m + r. The remainders r come mostly from a small
 * pool (some a, its complement 24 - a, 0 or 12, and one more), so that
 * complete days, ties and the self-pairing classes 0 and 12 come up often.
 * Small cases are short with m up to 3. Full-size ones have 100 lengths up
 * to 10^9 (sometimes the largest one with that remainder), in a shape the
 * seed picks: the pool, all multiples of 24 (every pair counts), all 12
 * modulo 24 (likewise), two complementary classes, or remainders 1 to 11
 * only (no pair counts). */
static int job_length(int r, int small)
{
    long long top = small ? 3 : (1000000000LL - r) / 24;
    long long m;

    if (!small && rnd_chance(10))
        m = top;
    else
        m = rnd(r == 0 ? 1 : 0, top);
    return (int)(24 * m + r);
}

void gen(int small)
{
    int hours[100], pool[4];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? 0 : (int)rnd(0, 4);
    int a = (int)rnd(1, 11), r, i;

    pool[0] = (int)rnd(0, 23);
    pool[1] = (24 - pool[0]) % 24;
    pool[2] = rnd_chance(50) ? 0 : 12;
    pool[3] = (int)rnd(0, 23);
    for (i = 0; i < n; i++) {
        if (shape == 1)
            r = 0;
        else if (shape == 2)
            r = 12;
        else if (shape == 3)
            r = rnd_chance(50) ? a : 24 - a;
        else if (shape == 4)
            r = (int)rnd(1, 11);
        else if (rnd_chance(15))
            r = (int)rnd(0, 23);
        else
            r = pool[rnd(0, 3)];
        hours[i] = job_length(r, small);
    }
    put_ints(hours, n);
}
