/* A permutation of 0 to n - 1, n from 1 to 8 when small and 1000 at full
 * size. The seed picks the shape: any random order, or one cycle through
 * every position in a random order (long cycles are where two steps
 * differ most from one step back), or, at full size only, a rotation
 * i -> i + k. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000, i, k;
    int shape = (int)rnd(0, small ? 1 : 2);
    int *nums = malloc((size_t)n * sizeof *nums);
    int *ord = malloc((size_t)n * sizeof *ord);

    for (i = 0; i < n; i++)
        ord[i] = i;
    rnd_shuffle(ord, n);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = ord[i];
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            nums[ord[i]] = ord[(i + 1) % n];
    } else {
        k = (int)rnd(1, n - 1);
        for (i = 0; i < n; i++)
            nums[i] = (i + k) % n;
    }
    put_ints(nums, n);
    free(ord);
    free(nums);
}
