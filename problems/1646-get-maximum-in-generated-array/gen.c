/* One n. Small cases are often tiny (0 to 12, where a[0] and a[1] matter
 * most) and otherwise anywhere in the range, which is small enough for
 * brute. Full-size ones sit at the top of the range: half of them at
 * n = 100, the rest from 85 (the first index holding the largest value,
 * 21) to 99. */
void gen(int small)
{
    if (small)
        put_int(rnd_chance(30) ? rnd(0, 12) : rnd(0, 100));
    else
        put_int(rnd_chance(50) ? 100 : rnd(85, 99));
}
