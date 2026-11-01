/* n from 1 to 8 when small and 1000 at full size. Small cases mostly use
 * distinct values from 1 to 1000, so any wrong order shows; one in four
 * uses values from 1 to 3, with repeats. At full size the seed picks a
 * shuffle of 1 to 1000 or random values from 1 to 1000. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, i, j, v;
    int repeats = small ? rnd_chance(25) : rnd_chance(50);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (repeats) {
            nums[i] = (int)rnd(1, small ? 3 : 1000);
        } else if (small) {
            do {
                v = (int)rnd(1, 1000);
                for (j = 0; j < i && nums[j] != v; j++)
                    ;
            } while (j < i);
            nums[i] = v;
        } else {
            nums[i] = i + 1;
        }
    }
    if (!small && !repeats)
        rnd_shuffle(nums, n);
    put_ints(nums, n);
    free(nums);
}
