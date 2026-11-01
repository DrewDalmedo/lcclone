/* Reference solution: trims never raise a tower, so a height below k
 * means -1. Otherwise only the tallest group can be cut, and at best down
 * to the next height present (or to k), so each trim removes one distinct
 * height above k: the answer is how many distinct heights exceed k. */
int minOperations(int *nums, int numsSize, int k)
{
    char seen[101] = {0};
    int count = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] < k)
            return -1;
        if (nums[i] > k && !seen[nums[i]]) {
            seen[nums[i]] = 1;
            count++;
        }
    }
    return count;
}
