/* Slow and obviously right: try every start of a 2k stretch and check
 * both halves element by element. */
static bool rises(const int *a, int len)
{
    int j;

    for (j = 1; j < len; j++)
        if (a[j] <= a[j - 1])
            return false;
    return true;
}

bool hasIncreasingSubarrays(int *nums, int numsSize, int k)
{
    int a;

    for (a = 0; a + 2 * k <= numsSize; a++)
        if (rises(nums + a, k) && rises(nums + a + k, k))
            return true;
    return false;
}
