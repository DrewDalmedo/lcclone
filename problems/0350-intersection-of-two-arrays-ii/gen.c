/* Two arrays of random values. Small cases draw from a very narrow range,
 * at the bottom or the top of 0..1000, so that values repeat a different
 * number of times in each array; full-size ones draw each array from its
 * own random range of up to 101 values, near the other's, so that the
 * ranges overlap in part and shared values repeat many times. */

static void fill(int *a, int n, int lo, int hi)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = (int)rnd(lo, hi);
}

void gen(int small)
{
    int n1 = small ? (int)rnd(1, 8) : 1000;
    int n2 = small ? (int)rnd(1, 8) : 1000;
    int *nums1 = malloc((size_t)n1 * sizeof *nums1);
    int *nums2 = malloc((size_t)n2 * sizeof *nums2);

    if (small) {
        int width = (int)rnd(0, 6);
        int lo = rnd_chance(30) ? 1000 - width : 0;

        fill(nums1, n1, lo, lo + width);
        fill(nums2, n2, lo, lo + width);
    } else {
        int lo1 = (int)rnd(0, 900);
        int lo2 = lo1 + (int)rnd(-60, 60);

        if (lo2 < 0)
            lo2 = 0;
        if (lo2 > 900)
            lo2 = 900;
        fill(nums1, n1, lo1, lo1 + (int)rnd(0, 100));
        fill(nums2, n2, lo2, lo2 + (int)rnd(0, 100));
    }
    put_ints(nums1, n1);
    put_ints(nums2, n2);
    free(nums1);
    free(nums2);
}
