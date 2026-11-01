/* Entries 0, 1 and 2. Small cases have 1 to 8 entries: zeros, with up to
 * three 1s and up to three 2s dropped at random places (a later one may
 * cover an earlier one); three times in four the seed asks for at least
 * one of each. Full-size ones have 100 entries, mostly zeros, and the
 * seed picks the shape: four 1s and 2s at random places, a single 1 and
 * a single 2 at the two ends, a closest pair at the far right with the 2
 * first, or 1s and no 2. */
void gen(int small)
{
    int nums[100], n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, small ? 7 : 3), i, k, ones, twos, first, second;

    for (i = 0; i < n; i++)
        nums[i] = 0;
    if (small) {
        ones = (int)rnd(shape >= 2, 3);
        twos = (int)rnd(shape >= 2, 3);
        for (k = 0; k < ones; k++) {
            i = (int)rnd(0, n - 1);
            nums[i] = 1;
        }
        for (k = 0; k < twos; k++) {
            i = (int)rnd(0, n - 1);
            nums[i] = 2;
        }
    } else if (shape == 0) {
        for (k = 0; k < 4; k++) {
            i = (int)rnd(0, n - 1);
            nums[i] = k % 2 + 1;
        }
    } else if (shape == 1) {
        first = (int)rnd(1, 2);
        nums[0] = first;
        nums[n - 1] = 3 - first;
    } else if (shape == 2) {
        first = (int)rnd(1, 2);
        second = 3 - first;
        for (i = 0; i < 60; i += 12)
            nums[i] = i % 24 == 0 ? first : second;
        k = (int)rnd(1, 9);
        nums[n - 1] = 1;
        nums[n - 1 - k] = 2;
    } else {
        for (i = 0; i < n; i++)
            if (rnd_chance(20))
                nums[i] = 1;
    }
    put_ints(nums, n);
}
