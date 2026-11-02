/* Small cases: up to 10 values from a range of random width, so trios, ties
 * and arrays with no trio all come up. Full-size cases have 100 values, in
 * one of three shapes picked by the seed: values from the whole range; a few
 * values repeated many times; or 50 values twice each (no trio), sometimes
 * with one of them moved to the last position so that a single trio ends
 * there. */
void gen(int small)
{
    int nums[100], perm[100];
    int n = small ? (int)rnd(1, 10) : 100, shape = (int)rnd(0, 3), i, top;

    if (small || shape <= 1) {
        top = small ? (int)rnd(1, n) : shape == 0 ? n : (int)rnd(1, 15);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
    } else {
        for (i = 0; i < n; i++)
            perm[i] = i + 1;
        rnd_shuffle(perm, n);
        for (i = 0; i < n; i++)
            nums[i] = perm[i / 2];
        rnd_shuffle(nums, n);
        if (shape == 3) {
            int q;

            do
                q = (int)rnd(0, n - 2);
            while (nums[q] == nums[n - 1]);
            nums[n - 1] = nums[q];
        }
    }
    put_ints(nums, n);
}
