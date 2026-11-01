/* An array with a chosen number of even elements: none, one, two, or any
 * number. Half the time the even values are 2 more than a multiple of 4,
 * so they have a single trailing zero. Small cases have 2 to 8 values of
 * at most 6 or 100, so that equal values come up often. Full-size ones
 * have 100 values from 1..100; the seed picks the shape, and the few
 * evens are often put at the far end, or at both ends. */
static int value(int even, int top)
{
    if (!even)
        return 2 * (int)rnd(0, (top - 1) / 2) + 1;
    if (rnd_chance(50))
        return 4 * (int)rnd(0, (top - 2) / 4) + 2;
    return 2 * (int)rnd(1, top / 2);
}

void gen(int small)
{
    int nums[100];
    int n = small ? (int)rnd(2, 8) : 100;
    int top = small && rnd_chance(50) ? 6 : 100;
    int shape = (int)rnd(0, 4), evens, place, i, t;

    evens = shape == 0 ? 0 : shape == 1 ? 1 : shape == 2 ? 2
          : (int)rnd(0, n);
    for (i = 0; i < n; i++)
        nums[i] = value(i < evens, top);
    place = (int)rnd(0, 2);
    if (place == 0) {
        rnd_shuffle(nums, n);
    } else {
        /* The evens at the end; with two, often one at each end. */
        for (i = 0; i < n / 2; i++) {
            t = nums[i];
            nums[i] = nums[n - 1 - i];
            nums[n - 1 - i] = t;
        }
        if (place == 2 && evens == 2) {
            t = nums[0];
            nums[0] = nums[n - 2];
            nums[n - 2] = t;
        }
    }
    put_ints(nums, n);
}
