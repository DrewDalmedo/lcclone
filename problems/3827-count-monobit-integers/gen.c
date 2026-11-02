/* One value of n. Small cases stay below 70, and full-size ones are from
 * 254 to 1000. Half of them land on, just below or just above an all-ones
 * number 2^k - 1, where the count changes (k up to 6 for small cases,
 * 8 or 9 for full-size ones); the rest are anywhere in their range. */
void gen(int small)
{
    int k, d, n;

    if (rnd_chance(50)) {
        k = small ? (int)rnd(1, 6) : (int)rnd(8, 9);
        d = (int)rnd(-1, 1);
        n = (1 << k) - 1 + d;
    } else {
        n = small ? (int)rnd(0, 69) : (int)rnd(500, 1000);
    }
    put_int(n);
}
