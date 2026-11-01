/* Slow and obviously right: sweep one group at a time, and after each
 * sweep compare every pair of the parcels left until no two match. */
static int has_repeat(const int *a, int n)
{
    int i, k;

    for (i = 0; i < n; i++)
        for (k = i + 1; k < n; k++)
            if (a[i] == a[k])
                return 1;
    return 0;
}

int minimumOperations(int *nums, int numsSize)
{
    int start = 0, sweeps = 0;

    while (has_repeat(nums + start, numsSize - start)) {
        start = start + 3 < numsSize ? start + 3 : numsSize;
        sweeps++;
    }
    return sweeps;
}
