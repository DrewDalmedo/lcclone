/* Reference solution: a table indexed by value records which arrays each
 * value occurs in (bit 1 for nums1, bit 2 for nums2); the values with both
 * bits set are the answer, collected in increasing order. */
int *intersection(int *nums1, int nums1Size, int *nums2, int nums2Size,
                  int *returnSize)
{
    unsigned char in[1001] = {0};
    int *out = malloc((size_t)(nums1Size < nums2Size ? nums1Size : nums2Size) *
                      sizeof *out);
    int n = 0, i, v;

    for (i = 0; i < nums1Size; i++)
        in[nums1[i]] |= 1;
    for (i = 0; i < nums2Size; i++)
        in[nums2[i]] |= 2;
    for (v = 0; v <= 1000; v++)
        if (in[v] == 3)
            out[n++] = v;
    *returnSize = n;
    return out;
}
