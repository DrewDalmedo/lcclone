/* Reference solution: count how often each value occurs in each array, in
 * tables indexed by value, then emit every value the smaller of its two
 * counts times, in increasing order. */
int *intersect(int *nums1, int nums1Size, int *nums2, int nums2Size,
               int *returnSize)
{
    int c1[1001] = {0}, c2[1001] = {0};
    int *out = malloc((size_t)(nums1Size < nums2Size ? nums1Size : nums2Size) *
                      sizeof *out);
    int n = 0, i, v, k;

    for (i = 0; i < nums1Size; i++)
        c1[nums1[i]]++;
    for (i = 0; i < nums2Size; i++)
        c2[nums2[i]]++;
    for (v = 0; v <= 1000; v++)
        for (k = c1[v] < c2[v] ? c1[v] : c2[v]; k > 0; k--)
            out[n++] = v;
    *returnSize = n;
    return out;
}
