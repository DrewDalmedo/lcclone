/* Small cases: a random k, then two lengths, both at most k, or one of
 * them (n or m) longer than k but at most 2k. Lengths of exactly k and 2k
 * come up often. Full-size cases have k = 10^5, and the seed picks the
 * lengths (seeds 1 to 4 make one of each): n = 2k and m = k (the largest
 * cost, k * k), m = 2k and n = k, n = k + 1 and m = k (the smallest cut,
 * cost k), or n = m = k (nothing to cut). Each has 10^10 or more pairs of
 * cuts, too many to try them all. */
static int length(int lo, int hi)
{
    if (rnd_chance(20))
        return rnd_chance(50) ? lo : hi;
    return (int)rnd(lo, hi);
}

void gen(int small)
{
    int k, which, n, m;

    if (!small) {
        k = 100000;
        which = (int)(gen_seed() % 4);
        put_int(which == 0 ? 2 * k : which == 2 ? k + 1 : k);
        put_int(which == 1 ? 2 * k : k);
        put_int(k);
        return;
    }
    k = (int)rnd(2, 6);
    which = (int)rnd(0, 2);
    n = which == 1 ? length(k + 1, 2 * k) : length(1, k);
    m = which == 2 ? length(k + 1, 2 * k) : length(1, k);
    put_int(n);
    put_int(m);
    put_int(k);
}
