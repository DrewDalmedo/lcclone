/* Reference solution: the ball bounces between the two sides of its
 * starting cell, taking one hit off each side in turn, and leaves the row
 * when the side it heads for is empty. So with sums L and R on the left
 * and right of an empty cell, both directions clear the row when L == R,
 * and only the one heading first for the larger side when they differ by
 * exactly 1. Carry L along and get R from the total. */
int countValidSelections(int *nums, int numsSize)
{
    int total = 0, left = 0, right, i, count = 0;

    for (i = 0; i < numsSize; i++)
        total += nums[i];
    for (i = 0; i < numsSize; i++) {
        if (nums[i] == 0) {
            right = total - left;
            if (left == right)
                count += 2;
            else if (left - right == 1 || right - left == 1)
                count += 1;
        }
        left += nums[i];
    }
    return count;
}
