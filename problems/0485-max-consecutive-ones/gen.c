/* An array of zeros and ones. Small cases have up to 12 elements, each a
 * 1 with a chance the seed picks, from none to all. Full-size ones have
 * 10^5 elements, in a shape the seed picks: all ones (the slowest input
 * for counting afresh from every position); mostly ones with a few zeros;
 * random, with the longest streak at the very end; alternating; or all
 * zeros. */
void gen(int small)
{
    static const int chance[] = {0, 30, 50, 70, 85, 100};
    int n = small ? (int)rnd(1, 12) : 100000, ones, tail, i;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small) {
        ones = chance[rnd(0, 5)];
        for (i = 0; i < n; i++)
            nums[i] = rnd_chance(ones);
    } else {
        switch (gen_seed() % 5) {
        case 0: /* all ones */
            for (i = 0; i < n; i++)
                nums[i] = 1;
            break;
        case 1: /* mostly ones: long streaks */
            ones = (int)rnd(90, 99);
            for (i = 0; i < n; i++)
                nums[i] = rnd_chance(ones);
            break;
        case 2: /* random, then a 0 and the longest streak at the end */
            tail = (int)rnd(1000, 50000);
            ones = (int)rnd(20, 80);
            for (i = 0; i < n - tail - 1; i++)
                nums[i] = rnd_chance(ones);
            nums[n - tail - 1] = 0;
            for (i = n - tail; i < n; i++)
                nums[i] = 1;
            break;
        case 3: /* alternating, starting with a 0 or a 1 */
            ones = (int)rnd(0, 1);
            for (i = 0; i < n; i++)
                nums[i] = (i + ones) % 2;
            break;
        default: /* all zeros */
            for (i = 0; i < n; i++)
                nums[i] = 0;
            break;
        }
    }
    put_ints(nums, n);
    free(nums);
}
