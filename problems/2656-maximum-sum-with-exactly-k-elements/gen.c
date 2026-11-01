/* Random counters and k. Small cases are tiny, since brute.c tries all
 * n^k sequences of picks: up to 4 counters with values up to 6 (so the
 * largest often repeats) and k up to 6, often more than n. Full-size cases
 * have 100 counters and k = 100 or a random k; the largest value lands
 * anywhere, and some cases reach the value 100. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 4) : 100;
    int k = small ? (int)rnd(1, 6) : (rnd_chance(50) ? 100 : (int)rnd(1, 100));
    int top = small ? 6 : (rnd_chance(30) ? 100 : (int)rnd(1, 100));
    int *nums = malloc((size_t)n * sizeof *nums);
    int i;

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(1, top);
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
