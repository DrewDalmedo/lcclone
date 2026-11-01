/* Slow and obviously right, by a different route: empty a copy of nums in
 * rounds, each time taking out one smallest and then one largest of the
 * values left (the same pairs as the sorted line), and record their average
 * as a double, which holds a .5 exactly. Then count the averages that differ
 * from every earlier one. */
int distinctAverages(int *nums, int numsSize)
{
    int *left = malloc((size_t)numsSize * sizeof *left);
    double *avg = malloc((size_t)(numsSize / 2) * sizeof *avg);
    int m = numsSize, rounds = 0, lo, hi, i, j, distinct = 0;

    for (i = 0; i < numsSize; i++)
        left[i] = nums[i];
    while (m > 0) {
        lo = 0;
        for (i = 1; i < m; i++)
            if (left[i] < left[lo])
                lo = i;
        avg[rounds] = left[lo];
        left[lo] = left[--m];
        hi = 0;
        for (i = 1; i < m; i++)
            if (left[i] > left[hi])
                hi = i;
        avg[rounds] = (avg[rounds] + left[hi]) / 2;
        left[hi] = left[--m];
        rounds++;
    }
    for (i = 0; i < rounds; i++) {
        for (j = 0; j < i; j++)
            if (avg[j] == avg[i])
                break;
        if (j == i)
            distinct++;
    }
    free(avg);
    free(left);
    return distinct;
}
