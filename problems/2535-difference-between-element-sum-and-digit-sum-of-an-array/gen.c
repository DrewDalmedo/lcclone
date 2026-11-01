/* One array. Small cases hold 1 to 8 numbers, often one-digit ones or
 * numbers full of zeros, nines or repeated digits, and often repeat an
 * earlier entry. Full-size ones hold 2000 numbers: any values, four-digit
 * values only, one-digit values only (the answer 0), or 2000 every time
 * (the largest answer). */
static const int special[] = {
    1, 9, 10, 99, 100, 101, 909, 999, 1000, 1009, 1090, 1111, 1999, 2000
};

static int value(void)
{
    int shape = (int)rnd(0, 3);

    if (shape == 0)
        return (int)rnd(1, 9);
    if (shape == 1)
        return special[rnd(0, 13)];
    return (int)rnd(1, 2000);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 2000;
    int shape = (int)rnd(0, 4), i;
    int *nums = malloc((size_t)n * sizeof *nums);

    for (i = 0; i < n; i++) {
        if (small) {
            if (i > 0 && rnd_chance(30))
                nums[i] = nums[rnd(0, i - 1)];
            else
                nums[i] = value();
        } else if (shape == 0) {
            nums[i] = 2000;
        } else if (shape == 1) {
            nums[i] = (int)rnd(1, 9);
        } else if (shape == 2) {
            nums[i] = (int)rnd(1000, 2000);
        } else {
            nums[i] = (int)rnd(1, 2000);
        }
    }
    put_ints(nums, n);
    free(nums);
}
