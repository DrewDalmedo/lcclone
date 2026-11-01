/* Small cases have 1, 2, 4, 8 or 16 values, full-size ones 1024. The seed
 * picks the range of the values: a few small ones (so that ties are
 * common), or anything up to 10^9; with "edges", about one value in ten
 * is 1 and another one in ten is 10^9. */
static int pick(int top, int edges)
{
    int r = (int)rnd(0, 9);

    if (edges && r == 0)
        return 1;
    if (edges && r == 1)
        return 1000000000;
    return (int)rnd(1, top);
}

void gen(int small)
{
    static const int tops[3] = {4, 20, 1000000000};
    int n = small ? 1 << (int)rnd(0, 4) : 1024;
    int shape = (int)rnd(0, 2), top, edges, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        top = tops[shape];
        edges = shape == 2 || rnd_chance(30);
    } else {
        top = shape == 1 ? 10 : 1000000000;
        edges = shape != 0;
    }
    for (i = 0; i < n; i++)
        nums[i] = pick(top, edges);
    put_ints(nums, n);
    free(nums);
}
