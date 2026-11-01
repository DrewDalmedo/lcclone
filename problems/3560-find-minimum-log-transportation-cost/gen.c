/* A random k, then two lengths: both at most k, or one of them (n or m)
 * longer than k but at most 2k. Lengths of exactly k and 2k come up often. */
static int length(int lo, int hi)
{
    if (rnd_chance(20))
        return rnd_chance(50) ? lo : hi;
    return (int)rnd(lo, hi);
}

void gen(int small)
{
    int k = small ? (int)rnd(2, 6) : (int)rnd(2, 100000);
    int which = (int)rnd(0, 2), n, m;

    if (!small && rnd_chance(25))
        k = 100000;
    n = which == 1 ? length(k + 1, 2 * k) : length(1, k);
    m = which == 2 ? length(k + 1, 2 * k) : length(1, k);
    put_int(n);
    put_int(m);
    put_int(k);
}
