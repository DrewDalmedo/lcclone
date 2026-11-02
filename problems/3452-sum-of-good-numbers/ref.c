/* Reference solution: one pass, comparing each element with the elements
 * k places to its left and right when those indices are inside the array,
 * and adding it when it is strictly greater than both. */
int sumOfGoodNumbers(int *nums, int numsSize, int k)
{
    int sum = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (i - k >= 0 && nums[i] <= nums[i - k])
            continue;
        if (i + k < numsSize && nums[i] <= nums[i + k])
            continue;
        sum += nums[i];
    }
    return sum;
}
