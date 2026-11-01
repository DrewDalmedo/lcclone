/* One position n. Small cases stay at n <= 25, where brute's plain
 * recursion is still quick, and often land on the starting terms.
 * Full-size ones sit at the top of the range, half of them at n = 37,
 * where plain recursion makes about 4 * 10^9 calls. */
void gen(int small)
{
    if (small)
        put_int(rnd_chance(25) ? rnd(0, 3) : rnd(0, 25));
    else
        put_int(rnd_chance(50) ? 37 : rnd(30, 36));
}
