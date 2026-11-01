/* Reads nums[numsSize], one past the end, before it looks at i. The
 * answer doesn't depend on what it reads, so only the sanitizers notice,
 * as LeetCode's AddressSanitizer does. */
int missing(int* nums, int numsSize) {
    int i;
    for (i = 0; i <= numsSize; i++)
        if (nums[i] != i || i == numsSize)
            return i;
    return -1;
}
