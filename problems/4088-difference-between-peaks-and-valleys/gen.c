/* Small cases: a few elements from a narrow band of values, near zero or
 * at either end of the range, so equal neighbours, negative peaks and
 * positive valleys come up often. Full size: 100 elements, shaped by the
 * seed: a zigzag where every inner index counts (at the extremes or with
 * random values), lone valleys or lone peaks kept apart by plateaus (a
 * negative result), random values over the whole or a narrow range, or a
 * sorted list. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

void gen(int small)
{
    int n = small ? (int)rnd(3, 9) : 100, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        int span = (int)rnd(1, 4);
        int where = (int)rnd(0, 3);
        int lo;

        lo = where == 0 ? -100 : where == 1 ? 100 - span : -2;
        for (i = 0; i < n; i++)
            nums[i] = lo + (int)rnd(0, span);
    } else {
        int shape = (int)rnd(0, 5);

        if (shape == 0) {
            int first = rnd_chance(50) ? 100 : -100;

            for (i = 0; i < n; i++)
                nums[i] = i % 2 ? -first : first;
        } else if (shape == 1) {
            /* Every third element differs from the equal ones around it. */
            int mid = (int)rnd(-99, 99), around;

            if (rnd_chance(50))
                around = (int)rnd(mid + 1, 100);
            else
                around = (int)rnd(-100, mid - 1);
            for (i = 0; i < n; i++)
                nums[i] = i % 3 == 1 ? mid : around;
        } else if (shape == 2) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(-100, 100);
        } else if (shape == 3) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(-2, 2);
        } else if (shape == 4) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(-100, 100);
            qsort(nums, (size_t)n, sizeof *nums, cmp_int);
        } else {
            int up = rnd_chance(50);

            for (i = 0; i < n; i++)
                if ((i % 2 == 0) == up)
                    nums[i] = (int)rnd(1, 100);
                else
                    nums[i] = (int)rnd(-100, 0);
        }
    }
    put_ints(nums, n);
    free(nums);
}
