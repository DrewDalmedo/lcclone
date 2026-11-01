/* Small cases: 2 to 8 values from 0 to 6, with a 0 or a 100 now and then,
 * so repeats and ties are common. Full-size cases hold 500 values from
 * 0 to 100: uniform, in decreasing order, a few distinct values repeated
 * many times, or small values with a single largest one at a random place
 * (its answer is 499). */
static int by_desc(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x < y) - (x > y);
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 500, i, shape, pool[3];
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        for (i = 0; i < n; i++) {
            if (rnd_chance(10))
                nums[i] = rnd_chance(50) ? 0 : 100;
            else
                nums[i] = (int)rnd(0, 6);
        }
    } else {
        shape = (int)rnd(0, 3);
        if (shape <= 1) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(0, 100);
            if (shape == 1)
                qsort(nums, (size_t)n, sizeof *nums, by_desc);
        } else if (shape == 2) {
            for (i = 0; i < 3; i++)
                pool[i] = (int)rnd(0, 100);
            for (i = 0; i < n; i++)
                nums[i] = pool[rnd(0, 2)];
        } else {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(0, 99);
            i = (int)rnd(0, n - 1);
            nums[i] = 100;
        }
    }
    put_ints(nums, n);
    free(nums);
}
