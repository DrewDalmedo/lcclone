/* Random pairs. Small cases have up to 4 pairs with counts and values of
 * 1 to 4, so that neighbouring pairs often share a value and counts often
 * equal values. Full-size ones have all 50 pairs and values of 1 to 100;
 * the seed picks counts of 100 everywhere (the longest result, 5000
 * numbers) or random counts. */
void gen(int small)
{
    int nums[100];
    int m = small ? (int)rnd(1, 4) : 50, all_max, i;

    all_max = small ? 0 : rnd_chance(50);
    for (i = 0; i < m; i++) {
        if (small)
            nums[2 * i] = (int)rnd(1, 4);
        else if (all_max)
            nums[2 * i] = 100;
        else
            nums[2 * i] = (int)rnd(1, 100);
        nums[2 * i + 1] = (int)rnd(1, small ? 4 : 100);
    }
    put_ints(nums, 2 * m);
}
