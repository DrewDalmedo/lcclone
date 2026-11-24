/* One position n. Small cases stay at n <= 25, where brute's plain
 * recursion is still quick, and often land on the starting terms.
 * Full-size ones are 37, 36 or 35, as the seed picks: the top of the
 * range, where the answer comes close to the int limit and plain
 * recursion makes about 4 * 10^9 calls, and just below it. */
void gen(int small)
{
    if (small)
        put_int(rnd_chance(25) ? rnd(0, 3) : rnd(0, 25));
    else
        put_int(37 - (long long)(gen_seed() % 3));
}
