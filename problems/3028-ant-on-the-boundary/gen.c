/* Hops for the ant. Small cases: up to 8 hops of length up to 3 (now and
 * then up to 10) that often jump straight back to the boundary when it is
 * in reach, so that returns from both sides come up often. Full-size
 * cases have 100 hops, in a shape the seed picks:
 *   0: random hops;
 *   1: random hops that often return when the boundary is in reach;
 *   2: x then -x, over and over (50 returns, the most there can be);
 *   3: a one-way run that stops exactly 256 units from the boundary,
 *      where a position kept in 8 bits would read 0, then hops mostly back
 *      towards the boundary, often landing on it once it is in reach. */
static int hop(int top)
{
    int h;

    do
        h = (int)rnd(-top, top);
    while (h == 0);
    return h;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? 1 : (int)(gen_seed() % 4);
    int top = (small && rnd_chance(80)) ? 3 : 10;
    int *nums = malloc((size_t)n * sizeof *nums);
    int pos = 0, i = 0, sign, h;

    if (shape == 3) {
        sign = rnd_chance(50) ? 1 : -1;
        while (pos * sign < 256) {
            h = (int)rnd(6, 10);
            if (h > 256 - pos * sign)
                h = 256 - pos * sign;
            nums[i++] = h * sign;
            pos += h * sign;
        }
    }
    for (; i < n; i++) {
        if (shape == 2 && i % 2 == 1) {
            h = -nums[i - 1];
        } else if (shape == 2 || shape == 0) {
            h = hop(top);
        } else if (pos != 0 && abs(pos) <= top) {
            h = rnd_chance(40) ? -pos : hop(top);
        } else if (shape == 3 && rnd_chance(80)) {
            h = (int)rnd(1, top);
            if (pos > 0)
                h = -h;
        } else {
            h = hop(top);
        }
        nums[i] = h;
        pos += h;
    }
    put_ints(nums, n);
    free(nums);
}
