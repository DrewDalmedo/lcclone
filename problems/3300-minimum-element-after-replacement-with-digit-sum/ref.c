/* Reference solution: peel off the decimal digits of each number with % 10
 * and / 10, adding them up, and keep the smallest sum. */
int minElement(int *nums, int numsSize)
{
    int best = INT_MAX, i, x, sum;

    for (i = 0; i < numsSize; i++) {
        sum = 0;
        for (x = nums[i]; x > 0; x /= 10)
            sum += x % 10;
        if (sum < best)
            best = sum;
    }
    return best;
}
