/* Slow and obviously right: add every row of both tallies into a table
 * indexed by id, then go through the ids 1 to 1000 in order and make a row
 * for each one that some tally listed. */
int **mergeArrays(int **nums1, int nums1Size, int *nums1ColSize,
                  int **nums2, int nums2Size, int *nums2ColSize,
                  int *returnSize, int **returnColumnSizes)
{
    int total[1001] = {0}, seen[1001] = {0};
    int cap = nums1Size + nums2Size, n = 0, i, id;
    int **out = malloc((size_t)cap * sizeof *out);

    (void)nums1ColSize;
    (void)nums2ColSize;
    for (i = 0; i < nums1Size; i++) {
        total[nums1[i][0]] += nums1[i][1];
        seen[nums1[i][0]] = 1;
    }
    for (i = 0; i < nums2Size; i++) {
        total[nums2[i][0]] += nums2[i][1];
        seen[nums2[i][0]] = 1;
    }
    *returnColumnSizes = malloc((size_t)cap * sizeof **returnColumnSizes);
    for (id = 1; id <= 1000; id++)
        if (seen[id]) {
            out[n] = malloc(2 * sizeof **out);
            out[n][0] = id;
            out[n][1] = total[id];
            (*returnColumnSizes)[n] = 2;
            n++;
        }
    *returnSize = n;
    return out;
}
