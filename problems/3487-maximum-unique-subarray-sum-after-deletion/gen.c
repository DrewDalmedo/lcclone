/* A random array. The seed picks how the values are drawn: from -top to
 * top; from -top to 0; negative only; from two or three values, so values
 * repeat a lot; mostly positive; or, at full size, the numbers 1 to n in a
 * random order, every one of them counted. Small cases have 1 to 10
 * entries from -6 to 6; full-size ones 100 entries from -100 to 100. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 100;
    int top = small ? 6 : 100;
    int shape = (int)rnd(0, small ? 4 : 5);
    int k = (int)rnd(2, 3), alpha[3], idx, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < 3; i++)
        alpha[i] = (int)rnd(-top, top);
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            nums[i] = (int)rnd(-top, top);
        } else if (shape == 1) {
            nums[i] = (int)rnd(-top, 0);
        } else if (shape == 2) {
            nums[i] = (int)rnd(-top, -1);
        } else if (shape == 3) {
            idx = (int)rnd(0, k - 1);
            nums[i] = alpha[idx];
        } else if (shape == 4) {
            nums[i] = (int)rnd(-top / 3, top);
        } else {
            nums[i] = i + 1;
        }
    }
    if (shape == 5)
        rnd_shuffle(nums, n);
    put_ints(nums, n);
    free(nums);
}
