/* Small cases: short lists of a few distinct values, often repeating the
 * previous value, so runs of equal neighbours come up all the time. Full
 * size: 100 elements, shaped by the seed: all equal, two values
 * alternating, runs of random lengths, or random values from a narrow or
 * the whole range. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 10) : 100, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        int top = (int)rnd(1, 4);
        int stick = (int)rnd(0, 70);

        nums[0] = (int)rnd(1, top);
        for (i = 1; i < n; i++)
            if (rnd_chance(stick))
                nums[i] = nums[i - 1];
            else
                nums[i] = (int)rnd(1, top);
    } else {
        int shape = (int)rnd(0, 4);
        int x = (int)rnd(1, 100);
        int y = (int)rnd(1, 99);

        if (y >= x)
            y++;
        i = 0;
        while (i < n) {
            if (shape == 2) {
                /* A run of up to 8 copies of one value. */
                int len = (int)rnd(1, 8), v = (int)rnd(1, 100);

                while (len-- > 0 && i < n)
                    nums[i++] = v;
                continue;
            }
            if (shape == 0)
                nums[i] = x;
            else if (shape == 1)
                nums[i] = i % 2 ? y : x;
            else if (shape == 3)
                nums[i] = (int)rnd(1, 100);
            else
                nums[i] = (int)rnd(1, 3);
            i++;
        }
    }
    put_ints(nums, n);
    free(nums);
}
