/* Two arrays of random values. Small cases draw from a narrow range, at
 * the bottom, around zero or at the top of -1000..1000, so that values
 * repeat and are shared often; full-size ones draw each array from its own
 * random range, so that the ranges overlap only in part. */

static void fill(int *a, int n, int lo, int hi)
{
    int i;

    for (i = 0; i < n; i++)
        a[i] = (int)rnd(lo, hi);
}

void gen(int small)
{
    int where = (int)rnd(0, 2), width = (int)rnd(0, 12);
    int n1 = small ? (int)rnd(1, 8) : 1000;
    int n2 = small ? (int)rnd(1, 8) : 1000;
    int *nums1 = malloc((size_t)n1 * sizeof *nums1);
    int *nums2 = malloc((size_t)n2 * sizeof *nums2);

    if (small) {
        int lo = where == 0 ? -1000 : where == 1 ? 1000 - width : -width / 2;

        fill(nums1, n1, lo, lo + width);
        fill(nums2, n2, lo, lo + width);
    } else {
        int lo1 = (int)rnd(-1000, 0), lo2 = (int)rnd(-1000, 0);

        fill(nums1, n1, lo1, lo1 + (int)rnd(500, 1000));
        fill(nums2, n2, lo2, lo2 + (int)rnd(500, 1000));
    }
    put_ints(nums1, n1);
    put_ints(nums2, n2);
    free(nums1);
    free(nums2);
}
