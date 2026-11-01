/* Slow and obviously right: make the swaps on a copy. Walk 1 to the left
 * end one swap at a time, then find n where it stands now and walk it to
 * the right end, counting every swap. */
int semiOrderedPermutation(int *nums, int numsSize)
{
    int a[50], n = numsSize, swaps = 0, i, t;

    for (i = 0; i < n; i++)
        a[i] = nums[i];
    for (;;) {
        for (i = 0; a[i] != 1; i++)
            ;
        if (i == 0)
            break;
        t = a[i - 1];
        a[i - 1] = a[i];
        a[i] = t;
        swaps++;
    }
    for (;;) {
        for (i = 0; a[i] != n; i++)
            ;
        if (i == n - 1)
            break;
        t = a[i + 1];
        a[i + 1] = a[i];
        a[i] = t;
        swaps++;
    }
    return swaps;
}
