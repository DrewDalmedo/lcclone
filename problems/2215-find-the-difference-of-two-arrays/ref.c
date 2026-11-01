/* Reference solution: a table indexed by value + 1000 records which arrays
 * each value occurs in (bit 1 for nums1, bit 2 for nums2); the values with
 * only bit 1 set make the first array and those with only bit 2 the
 * second, collected in increasing order. */
int **findDifference(int *nums1, int nums1Size, int *nums2, int nums2Size,
                     int *returnSize, int **returnColumnSizes)
{
    unsigned char in[2001] = {0};
    int **out = malloc(2 * sizeof *out);
    int *cols = malloc(2 * sizeof *cols);
    int i, v;

    for (i = 0; i < nums1Size; i++)
        in[nums1[i] + 1000] |= 1;
    for (i = 0; i < nums2Size; i++)
        in[nums2[i] + 1000] |= 2;
    out[0] = malloc((size_t)nums1Size * sizeof **out);
    out[1] = malloc((size_t)nums2Size * sizeof **out);
    cols[0] = cols[1] = 0;
    for (v = 0; v <= 2000; v++) {
        if (in[v] == 1)
            out[0][cols[0]++] = v - 1000;
        else if (in[v] == 2)
            out[1][cols[1]++] = v - 1000;
    }
    *returnSize = 2;
    *returnColumnSizes = cols;
    return out;
}
