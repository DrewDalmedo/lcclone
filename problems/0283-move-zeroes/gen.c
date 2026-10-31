/* Random values with plenty of zeros: each element is zero with a chance
 * picked per case, from none to all, and otherwise any int (small values
 * for small cases, with the int extremes now and then). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000, i;
    int zeros = (int)rnd(0, 100);
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (rnd_chance(zeros))
            nums[i] = 0;
        else if (rnd_chance(5))
            nums[i] = rnd_chance(50) ? INT_MIN : INT_MAX;
        else if (small)
            nums[i] = (int)rnd(-9, 9);
        else
            nums[i] = (int)rnd(INT_MIN, INT_MAX);
    }
    put_ints(nums, n);
    free(nums);
}
