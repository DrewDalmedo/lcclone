/* One n. Small cases are mostly small n, or n at the edges where a
 * numeral gains a digit (16, 256, 393, ...); full-size ones are large n,
 * often 1000. */
void gen(int small)
{
    static const int edge[] = {1, 2, 15, 16, 17, 35, 37, 255, 256, 257,
                               392, 393, 394, 999, 1000};
    int pick = (int)rnd(1, 10), n;

    if (small)
        n = pick <= 5 ? (int)rnd(1, 40)
            : pick <= 8 ? edge[rnd(0, 14)] : (int)rnd(1, 1000);
    else
        n = pick <= 3 ? 1000 : (int)rnd(393, 1000);
    put_int(n);
}
