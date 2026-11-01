/* Positive numbers and -1 entries. The seed picks the chance of a -1, from
 * none at all to every entry, so that long streaks, streaks that run past
 * the start of the log and arrays without a -1 all come up. Small cases
 * use the numbers 1 to 4, so that values repeat in the log; full-size
 * ones have 100 entries with numbers up to 100. */
void gen(int small)
{
    static const int chances[] = {0, 20, 40, 50, 60, 80, 90, 100};
    int n = small ? (int)rnd(1, 10) : 100;
    int top = small ? 4 : 100;
    int chance = chances[rnd(0, 7)];
    int nums[100], i;

    for (i = 0; i < n; i++)
        nums[i] = rnd_chance(chance) ? -1 : (int)rnd(1, top);
    put_ints(nums, n);
}
