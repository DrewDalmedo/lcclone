/* Slow and obviously right: multiply every three entries at three
 * different indices, and keep the largest product. */
int maximumProduct(int *nums, int numsSize)
{
    long long best = 0, p;
    int i, j, k, first = 1;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++) {
                p = (long long)nums[i] * nums[j] * nums[k];
                if (first || p > best)
                    best = p;
                first = 0;
            }
    return (int)best;
}
