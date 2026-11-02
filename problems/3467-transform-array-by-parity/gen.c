/* An array of positive integers. Each element is even with a chance the
 * seed picks, from none to all, so all-even and all-odd arrays come up.
 * Small cases have 1 to 10 elements up to 20; full-size ones have 100
 * elements up to 1000, and some use only the bounds 1 and 1000. */
void gen(int small)
{
    static const int chance[] = {0, 20, 50, 80, 100};
    int n = small ? (int)rnd(1, 10) : 100, i, even, v;
    int top = small ? 10 : 500, bounds = !small && rnd_chance(25);
    int *nums = malloc((size_t)n * sizeof *nums);

    even = chance[rnd(0, 4)];
    for (i = 0; i < n; i++) {
        if (bounds) {
            nums[i] = rnd_chance(even) ? 1000 : 1;
        } else {
            v = (int)rnd(1, top);
            nums[i] = rnd_chance(even) ? 2 * v : 2 * v - 1;
        }
    }
    put_ints(nums, n);
    free(nums);
}
