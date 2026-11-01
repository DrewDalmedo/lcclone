/* Reference solution: nums[0] always starts the first piece, and any two
 * different positions after it can start the other two. So the answer is
 * nums[0] plus the two smallest entries of nums[1..n-1], found in one
 * pass (two equal entries may both be used). */
int minimumCost(int *nums, int numsSize)
{
    int first = INT_MAX, second = INT_MAX, i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] < first) {
            second = first;
            first = nums[i];
        } else if (nums[i] < second) {
            second = nums[i];
        }
    }
    return nums[0] + first + second;
}
