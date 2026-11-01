/* Reference solution: walk from the right, keeping the sum of the elements
 * already passed. Index i is dominant when nums[i] times the number of
 * elements to its right exceeds that sum, which compares with the exact
 * average without dividing. */
int dominantIndices(int *nums, int numsSize)
{
    long long suffix = 0;
    int count = 0, i, right;

    for (i = numsSize - 1; i >= 0; i--) {
        right = numsSize - 1 - i;
        if (right > 0 && (long long)nums[i] * right > suffix)
            count++;
        suffix += nums[i];
    }
    return count;
}
