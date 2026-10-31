/* Two sorted runs: m values at the front of nums1, followed by n zeros,
 * and n values in nums2. Small cases use values from -5 to 5, so real
 * zeros, negatives and values shared by both arrays come up often, and
 * now and then the extremes -10^9 and 10^9. */
static int pick(int small, int extremes)
{
    if (!small)
        return (int)rnd(-1000000000, 1000000000);
    if (extremes && rnd_chance(40))
        return rnd_chance(50) ? -1000000000 : 1000000000;
    return (int)rnd(-5, 5);
}

static void sort(int *a, int len)
{
    int i, j, v;

    for (i = 1; i < len; i++) {
        v = a[i];
        for (j = i; j > 0 && a[j - 1] > v; j--)
            a[j] = a[j - 1];
        a[j] = v;
    }
}

void gen(int small)
{
    int m, n, i, extremes = rnd_chance(20);
    int nums1[200], nums2[200];

    if (small) {
        m = (int)rnd(0, 4);
        n = (int)rnd(m == 0 ? 1 : 0, 4);
    } else {
        m = rnd_chance(10) ? (rnd_chance(50) ? 0 : 200) : (int)rnd(1, 199);
        n = 200 - m;
    }
    for (i = 0; i < m; i++)
        nums1[i] = pick(small, extremes);
    sort(nums1, m);
    for (i = 0; i < n; i++)
        nums1[m + i] = 0;
    for (i = 0; i < n; i++)
        nums2[i] = pick(small, extremes);
    sort(nums2, n);
    put_ints(nums1, m + n);
    put_int(m);
    put_ints(nums2, n);
    put_int(n);
}
