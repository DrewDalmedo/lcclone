/* Reference solution: in increasing order the smallest value comes
 * first, and it sits at index p now, so the only candidate is the
 * (n - p) % n right shifts that bring it to index 0. Walk once around
 * the array from index p, as if its ends were joined; the candidate works
 * exactly when every value on the way is smaller than the next. */
int minimumRightShifts(int *nums, int numsSize)
{
    int n = numsSize, p = 0, i;

    for (i = 1; i < n; i++)
        if (nums[i] < nums[p])
            p = i;
    for (i = 0; i + 1 < n; i++)
        if (nums[(p + i) % n] > nums[(p + i + 1) % n])
            return -1;
    return (n - p) % n;
}
