/* Reference solution: one pass, keeping the best element so far. An
 * element replaces it when it is nearer to zero, or equally near and
 * larger (which means it is the positive one of d and -d). */
int findClosestNumber(int *nums, int numsSize)
{
    int best = nums[0], i, a, b;

    for (i = 1; i < numsSize; i++) {
        a = abs(nums[i]);
        b = abs(best);
        if (a < b || (a == b && nums[i] > best))
            best = nums[i];
    }
    return best;
}
