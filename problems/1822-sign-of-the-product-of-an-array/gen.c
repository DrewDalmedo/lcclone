/* Random arrays. Small cases hold a few values from -3 to 3, half of
 * them with no zero at all. Full-size ones hold 1000 values, in a shape
 * the seed picks: any values from -100 to 100; no zero, with random
 * signs; +-100 throughout with one zero near the end (a double product
 * turns into infinity, then NaN); or powers of two only, with random
 * signs (a fixed-width product wraps to exactly 0). */
void gen(int small)
{
    static const int pow2[] = {1, 2, 4, 8, 16, 32, 64};
    int n = small ? (int)rnd(1, 8) : 1000, i, v;
    int shape = small ? (int)rnd(0, 1) : (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (shape == 0) {
            v = small ? (int)rnd(-3, 3) : (int)rnd(-100, 100);
        } else {
            if (small)
                v = (int)rnd(1, 3);
            else if (shape == 1)
                v = (int)rnd(1, 100);
            else if (shape == 2)
                v = 100;
            else
                v = pow2[rnd(0, 6)];
            if (rnd_chance(50))
                v = -v;
        }
        nums[i] = v;
    }
    if (shape == 2)
        nums[rnd(n - 10, n - 1)] = 0;
    put_ints(nums, n);
    free(nums);
}
