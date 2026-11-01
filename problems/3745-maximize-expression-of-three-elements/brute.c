/* Slow and obviously right: try every a, b and c at three different
 * positions and keep the largest a + b - c. */
int maximizeExpressionOfThree(int *nums, int numsSize)
{
    int best = 0, found = 0, i, j, k, value;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++)
            for (k = 0; k < numsSize; k++) {
                if (i == j || j == k || i == k)
                    continue;
                value = nums[i] + nums[j] - nums[k];
                if (!found || value > best)
                    best = value;
                found = 1;
            }
    return best;
}
