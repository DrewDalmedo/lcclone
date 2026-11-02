/* Slow and obviously right: for each element, look at every other index
 * and check the ones at distance exactly k. */
int sumOfGoodNumbers(int *nums, int numsSize, int k)
{
    int sum = 0, i, j, good;

    for (i = 0; i < numsSize; i++) {
        good = 1;
        for (j = 0; j < numsSize; j++)
            if ((j - i == k || i - j == k) && !(nums[i] > nums[j]))
                good = 0;
        if (good)
            sum += nums[i];
    }
    return sum;
}
