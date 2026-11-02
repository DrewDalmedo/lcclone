/* Reference solution: add the two largest entries and subtract the
 * smallest. One pass keeps the largest and second largest entries (a
 * repeated maximum fills both) and the smallest; with at least three
 * entries, those are three different positions. */
int maximizeExpressionOfThree(int *nums, int numsSize)
{
    int first = nums[0], second = nums[1], low = nums[0], i;

    if (second > first) {
        first = nums[1];
        second = nums[0];
    }
    if (nums[1] < low)
        low = nums[1];
    for (i = 2; i < numsSize; i++) {
        if (nums[i] > first) {
            second = first;
            first = nums[i];
        } else if (nums[i] > second) {
            second = nums[i];
        }
        if (nums[i] < low)
            low = nums[i];
    }
    return first + second - low;
}
