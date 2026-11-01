/* Small cases: up to 8 towers, k from 1 to 6, heights at most 4 above k
 * (so heights repeat), and now and then a tower below k. Full-size cases
 * have 100 towers; the seed picks the shape: every height 1 to 100 once
 * with k = 1 (the largest answer, 99); random heights from k to 100;
 * the same with one tower below k at the very end; or few distinct
 * heights, each repeated many times. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, k, v, top;
    int shape = small ? 0 : (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        k = (int)rnd(1, 6);
        for (i = 0; i < n; i++) {
            if (k > 1 && rnd_chance(12))
                v = (int)rnd(1, k - 1);
            else
                v = (int)rnd(k, k + 4);
            nums[i] = v;
        }
    } else if (shape == 0) {
        k = 1;
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else {
        k = (int)rnd(shape == 2 ? 2 : 1, 100);
        top = shape == 3 ? (int)rnd(k, k + 5 < 100 ? k + 5 : 100) : 100;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(k, top);
        if (shape == 2)
            nums[n - 1] = (int)rnd(1, k - 1);
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
