/* Random values with plenty of zeros: each element is zero with a chance
 * picked per case, from none to all, and otherwise any int (small values
 * for small cases, with the int extremes now and then). Full-size cases
 * have 10^4 elements, and the seed picks the shape: zeros at random, zeros
 * filling the first half (the most moving for one zero at a time), no
 * zero at all, or nothing but zeros. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000, i;
    int zeros = (int)rnd(0, 100), shape = small ? 0 : (int)(gen_seed() % 4);
    int *nums = malloc((size_t)n * sizeof *nums);

    if (shape >= 2)
        zeros = shape == 2 ? 0 : 100;
    for (i = 0; i < n; i++) {
        if (shape == 1 ? i < n / 2 : rnd_chance(zeros))
            nums[i] = 0;
        else if (rnd_chance(5))
            nums[i] = rnd_chance(50) ? INT_MIN : INT_MAX;
        else if (small)
            nums[i] = (int)rnd(-9, 9);
        else
            nums[i] = (int)rnd(INT_MIN, INT_MAX);
        if (shape == 1 && i >= n / 2 && nums[i] == 0)
            nums[i] = 1;
    }
    put_ints(nums, n);
    free(nums);
}
