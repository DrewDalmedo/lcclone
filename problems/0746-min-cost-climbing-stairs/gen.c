/* One list of fees. Small cases have 2 to 16 pads (few enough for brute.c
 * to try every route) with fees of 0 to 9, so that ties are common, or
 * fees that are each 0 or 999, or all one value. Full-size ones have 1000
 * pads, the most allowed, with fees of 0 to 999, all 999 (the largest
 * answer), each 0 or 999, or rising along the course. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 16) : 1000, i;
    int shape = (int)rnd(0, 9);
    int same = (int)rnd(0, 999);
    int *cost = malloc((size_t)n * sizeof *cost);

    for (i = 0; i < n; i++) {
        if (shape < 5)
            cost[i] = (int)rnd(0, small ? 9 : 999);
        else if (shape < 7)
            cost[i] = rnd_chance(50) ? 0 : 999;
        else if (shape < 9)
            cost[i] = small ? same : 999;
        else
            cost[i] = (int)(i * 999LL / (n - 1));
    }
    put_ints(cost, n);
    free(cost);
}
