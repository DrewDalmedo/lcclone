/* Small cases: up to 8 values drawn from a pool of 1 to 4 values, which
 * now and then include the bounds 1 and 100, so that copies, ties between
 * the largest counts and adjacent equal values come up often. Full-size
 * cases have 100 values, in a shape picked by the seed: a shuffle of 1 to
 * 100 (one round), one value 100 times (100 rounds), one value 99 times
 * beside another, or random values from a pool of 2 to 100 values. */
void gen(int small)
{
    int nums[100], pool[100], n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? 3 : (int)rnd(0, 3), size, i, v, at;

    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else if (shape == 1) {
        v = (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 2) {
        v = (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
        at = (int)rnd(0, n - 1);
        v = (int)rnd(1, 99);
        nums[at] = v < nums[0] ? v : v + 1;
    } else {
        size = small ? (int)rnd(1, 4) : (int)rnd(2, 100);
        for (i = 0; i < size; i++) {
            v = (int)rnd(0, 9);
            pool[i] = v == 0 ? 1 : v == 1 ? 100 : (int)rnd(1, 100);
        }
        for (i = 0; i < n; i++)
            nums[i] = pool[rnd(0, size - 1)];
    }
    put_ints(nums, n);
}
