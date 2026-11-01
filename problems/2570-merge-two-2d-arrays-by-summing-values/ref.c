/* Reference solution: both tallies are sorted by id, so merge them with
 * two pointers, as in the merge step of merge sort. When the same id is at
 * the front of both, its two values make one row. Whatever is left of one
 * tally once the other runs out is copied as it is. */
int **mergeArrays(int **nums1, int nums1Size, int *nums1ColSize,
                  int **nums2, int nums2Size, int *nums2ColSize,
                  int *returnSize, int **returnColumnSizes)
{
    int cap = nums1Size + nums2Size, i = 0, j = 0, n = 0, id, val;
    int **out = malloc((size_t)cap * sizeof *out);

    (void)nums1ColSize;
    (void)nums2ColSize;
    *returnColumnSizes = malloc((size_t)cap * sizeof **returnColumnSizes);
    while (i < nums1Size || j < nums2Size) {
        if (j == nums2Size || (i < nums1Size && nums1[i][0] < nums2[j][0])) {
            id = nums1[i][0];
            val = nums1[i][1];
            i++;
        } else if (i == nums1Size || nums2[j][0] < nums1[i][0]) {
            id = nums2[j][0];
            val = nums2[j][1];
            j++;
        } else {
            id = nums1[i][0];
            val = nums1[i][1] + nums2[j][1];
            i++;
            j++;
        }
        out[n] = malloc(2 * sizeof **out);
        out[n][0] = id;
        out[n][1] = val;
        (*returnColumnSizes)[n] = 2;
        n++;
    }
    *returnSize = n;
    return out;
}
