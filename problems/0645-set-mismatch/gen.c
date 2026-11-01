/* The integers 1 to n, with the lost one left out and the extra one in
 * twice. The lost one is 1 a fifth of the time, n a fifth of the time,
 * and otherwise any of 1 to n; the extra one is just below it a fourth of
 * the time, just above it a fourth of the time, and otherwise any other
 * integer of the range. The seed picks the order: shuffled (half the
 * time), increasing, or decreasing. Small cases have n from 2 to 9;
 * full-size ones n = 10^4, where a fourth of the time both integers are
 * within 9 of n (the slowest for counting each integer in turn). */
void gen(int small)
{
    int n = small ? (int)rnd(2, 9) : 10000, extra, lost, order, i, k, t;
    int *nums = malloc((size_t)n * sizeof *nums);

    if (!small && rnd_chance(25)) {
        lost = (int)rnd(n - 9, n);
        do
            extra = (int)rnd(n - 9, n);
        while (extra == lost);
    } else {
        switch (rnd(0, 4)) {
        case 0:
            lost = 1;
            break;
        case 1:
            lost = n;
            break;
        default:
            lost = (int)rnd(1, n);
        }
        switch (rnd(0, 3)) {
        case 0:
            extra = lost - 1;
            break;
        case 1:
            extra = lost + 1;
            break;
        default:
            extra = lost;
        }
        while (extra < 1 || extra > n || extra == lost)
            extra = (int)rnd(1, n);
    }
    k = 0;
    for (i = 1; i <= n; i++) {
        if (i != lost)
            nums[k++] = i;
        if (i == extra)
            nums[k++] = i;
    }
    order = (int)rnd(0, 3);
    if (order <= 1)
        rnd_shuffle(nums, n);
    else if (order == 3)
        for (i = 0; i < n / 2; i++) {
            t = nums[i];
            nums[i] = nums[n - 1 - i];
            nums[n - 1 - i] = t;
        }
    put_ints(nums, n);
    free(nums);
}
