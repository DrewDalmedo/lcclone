/* Small cases: up to 8 values from 0 to a random top of at most 10, so
 * that duplicates, zeros and values above n are common. Full-size ones
 * have 100 values from 0 to 1000, in one of four shapes picked by the
 * seed: random values; a planted match x, with x values >= x (some equal
 * to x) and the rest below it; no match because of copies of one value v
 * right at the boundary (more than v elements >= v, at most v elements
 * >= v + 1); or every value >= 100, so that the match is 100, a number that
 * may not be in the array. */
void gen(int small)
{
    int nums[100], n, shape, x, v, m, e, i, k, lo;

    if (small) {
        int top = (int)rnd(0, 10);

        n = (int)rnd(1, 8);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, top);
        put_ints(nums, n);
        return;
    }
    n = 100;
    shape = (int)rnd(0, 3);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, 1000);
    } else if (shape == 1) {
        x = (int)rnd(1, n);
        for (i = 0; i < n; i++) {
            if (i < x) {
                lo = x;
                nums[i] = rnd_chance(25) ? lo : (int)rnd(lo, 1000);
            } else {
                nums[i] = (int)rnd(0, x - 1);
            }
        }
    } else if (shape == 2) {
        v = (int)rnd(1, n - 1);
        m = (int)rnd(0, v);
        e = (int)rnd(v - m + 1, n - m);
        k = 0;
        for (i = 0; i < m; i++)
            nums[k++] = (int)rnd(v + 1, 1000);
        for (i = 0; i < e; i++)
            nums[k++] = v;
        while (k < n)
            nums[k++] = (int)rnd(0, v - 1);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(100, 1000);
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
