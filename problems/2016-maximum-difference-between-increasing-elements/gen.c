/* Small cases have 2 to 8 numbers from a narrow range (1 to 6, just below
 * 10^9, or only 1 and 10^9), so that ties come up often; the seed may
 * also sort them falling (no rise: -1), make them all equal, or put the
 * largest number first and the smallest last. Full-size cases have 1000
 * numbers up to 10^9, in a shape the seed picks: random; never rising,
 * with repeats (the answer is -1); the largest number first and the
 * smallest last, with smaller rises between; or falling all the way to 1
 * and then 10^9 at the very end (the largest answer, at the last
 * element). */
#define TOP 1000000000

static int falling(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 1000, i, range, shape, lo, hi;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        range = (int)rnd(0, 3);
        lo = range == 2 ? TOP - 5 : 1;
        hi = range == 2 || range == 3 ? TOP : 6;
        shape = (int)rnd(0, 7);
        for (i = 0; i < n; i++)
            if (range == 3)
                nums[i] = rnd_chance(50) ? lo : hi;
            else
                nums[i] = (int)rnd(lo, hi);
        if (shape == 5) {
            qsort(nums, (size_t)n, sizeof *nums, falling);
        } else if (shape == 6) {
            for (i = 1; i < n; i++)
                nums[i] = nums[0];
        } else if (shape == 7) {
            nums[0] = hi;
            nums[n - 1] = lo;
        }
    } else {
        shape = (int)(gen_seed() % 4);
        for (i = 0; i < n; i++)
            if (shape == 0)
                nums[i] = (int)rnd(1, TOP);
            else if (shape == 1)
                nums[i] = (int)rnd(1, 1000);
            else
                nums[i] = (int)rnd(2, TOP - 1);
        if (shape == 1 || shape == 3)
            qsort(nums, (size_t)n, sizeof *nums, falling);
        if (shape == 2) {
            nums[0] = TOP;
            nums[n - 1] = 1;
        } else if (shape == 3) {
            nums[n - 2] = 1;
            nums[n - 1] = TOP;
        }
    }
    put_ints(nums, n);
    free(nums);
}
