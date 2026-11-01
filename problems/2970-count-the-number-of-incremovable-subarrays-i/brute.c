/* Slow and obviously right: cut out every stretch l..r in turn, copy what
 * is left into a new array, and check that it rises strictly. */
int incremovableSubarrayCount(int *nums, int numsSize)
{
    int left[50];
    int n = numsSize, count = 0, l, r, i, m, ok;

    for (l = 0; l < n; l++)
        for (r = l; r < n; r++) {
            m = 0;
            for (i = 0; i < n; i++)
                if (i < l || i > r)
                    left[m++] = nums[i];
            ok = 1;
            for (i = 1; i < m; i++)
                if (left[i - 1] >= left[i])
                    ok = 0;
            count += ok;
        }
    return count;
}
