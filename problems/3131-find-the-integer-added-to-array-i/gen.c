/* Picks x first, then values for nums1 that stay from 0 to the top after
 * adding x, and shuffles nums1 + x into nums2. Small cases have up to 8
 * values from 0 to 10, so duplicates are common; full-size ones have 100
 * values up to 1000, and the seed sometimes makes x +-1000 (every value 0
 * or 1000) or 0. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 10 : 1000;
    int nums1[100], nums2[100], x, lo, hi, shape, i;

    shape = (int)rnd(0, 15);
    if (shape == 0)
        x = top;
    else if (shape == 1)
        x = -top;
    else if (shape == 2)
        x = 0;
    else
        x = (int)rnd(-top, top);
    lo = x < 0 ? -x : 0;
    hi = x > 0 ? top - x : top;
    for (i = 0; i < n; i++) {
        nums1[i] = (int)rnd(lo, hi);
        nums2[i] = nums1[i] + x;
    }
    rnd_shuffle(nums2, n);
    put_ints(nums1, n);
    put_ints(nums2, n);
}
