/* nums holds 2n values, n from 1 to 4 when small and 500 at full size.
 * The values are usually all different, so any wrong order shows: at full
 * size always (a shuffle of 1 to 1000), when small three times in four
 * (distinct values from 1 to 1000). The other small cases use values from
 * 1 to 3, with repeats. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 4) : 500, m = 2 * n, i, j, v, repeats;
    int *nums = malloc((size_t)m * sizeof *nums);

    repeats = small && rnd_chance(25);
    for (i = 0; i < m; i++) {
        if (!small) {
            nums[i] = i + 1;
        } else if (repeats) {
            nums[i] = (int)rnd(1, 3);
        } else {
            do {
                v = (int)rnd(1, 1000);
                for (j = 0; j < i && nums[j] != v; j++)
                    ;
            } while (j < i);
            nums[i] = v;
        }
    }
    if (!small)
        rnd_shuffle(nums, m);
    put_ints(nums, m);
    put_int(n);
    free(nums);
}
