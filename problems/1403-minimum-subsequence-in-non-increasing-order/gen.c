/* Small cases: up to 10 items of value 1 to 6, so that ties at exactly
 * half the total and repeated values are common; sometimes all equal, or
 * one item close to the sum of the rest. Full-size cases: 500 items,
 * random, all equal, nearly equal (99 or 100), or sorted ascending, the
 * shapes where the answer is long. */
static int cmp_up(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 500;
    int top = small ? 6 : 100;
    int shape = (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);
    int i, v, rest = 0, big;

    if (shape == 1) {
        /* All equal. */
        v = (int)rnd(1, top);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 2 && small) {
        /* One item close to the sum of all the others. */
        for (i = 1; i < n; i++) {
            nums[i] = (int)rnd(1, 3);
            rest += nums[i];
        }
        big = rest + (int)rnd(-1, 1);
        if (big < 1)
            big = 1;
        if (big > 100)
            big = 100;
        nums[0] = big;
        rnd_shuffle(nums, n);
    } else if (shape == 2) {
        /* Nearly equal. */
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(99, 100);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
        if (shape == 3 && !small)
            qsort(nums, (size_t)n, sizeof *nums, cmp_up);
    }
    put_ints(nums, n);
    free(nums);
}
