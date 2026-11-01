/* Small cases: up to 8 squares and moves from -20 to 20, so that tokens
 * go round the loop several times either way, with zeros now and then.
 * Full-size cases have 100 squares and moves from -100 to 100; the seed
 * may favour long moves (near +-100) or put zeros in. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, v;
    int shape = small ? 0 : (int)rnd(0, 2);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (small)
            v = rnd_chance(15) ? 0 : (int)rnd(-20, 20);
        else if (shape == 1) {
            v = (int)rnd(90, 100);
            if (rnd_chance(50))
                v = -v;
        } else if (shape == 2)
            v = rnd_chance(30) ? 0 : (int)rnd(-100, 100);
        else
            v = (int)rnd(-100, 100);
        nums[i] = v;
    }
    put_ints(nums, n);
    free(nums);
}
