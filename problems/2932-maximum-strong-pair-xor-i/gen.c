/* Random arrays; the seed picks the shape:
 * 0: values from a narrow range, so duplicates and ties are common;
 * 1: values from the whole range, in descending order;
 * 2: pairs x, y with y exactly 2 * x or just past it (2 * x + 1), in a
 *    random order, so the boundary of a strong pair decides;
 * 3: values from a chain that more than doubles at each step, so the only
 *    strong pairs are equal values and the answer is 0.
 * Small cases have up to 8 values of at most 20; full-size ones have 50
 * values of up to 100. */
static int cmp_desc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50, shape = (int)rnd(0, 3);
    int top = small ? 20 : 100, nums[50], chain[8], len = 0, i, x, d;

    if (shape == 3) {
        chain[len++] = (int)rnd(1, 3);
        for (;;) {
            d = (int)rnd(1, 3);
            x = 2 * chain[len - 1] + d;
            if (x > top)
                break;
            chain[len++] = x;
        }
    }
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            nums[i] = (int)rnd(1, small ? 6 : 12);
            if (!small)
                nums[i] += 88;
        } else if (shape == 1) {
            nums[i] = (int)rnd(1, top);
        } else if (shape == 2) {
            if (i % 2 == 0) {
                nums[i] = (int)rnd(1, (top - 1) / 2);
            } else {
                d = (int)rnd(0, 1);
                nums[i] = 2 * nums[i - 1] + d;
            }
        } else {
            nums[i] = chain[rnd(0, len - 1)];
        }
    }
    if (shape == 1)
        qsort(nums, (size_t)n, sizeof nums[0], cmp_desc);
    if (shape == 2)
        rnd_shuffle(nums, n);
    put_ints(nums, n);
}
