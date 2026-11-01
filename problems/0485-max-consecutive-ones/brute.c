/* Slow and obviously right: from every position, count how many ones
 * follow in a row, and keep the largest count. */
int findMaxConsecutiveOnes(int *nums, int numsSize)
{
    int best = 0, start, len;

    for (start = 0; start < numsSize; start++) {
        len = 0;
        while (start + len < numsSize && nums[start + len] == 1)
            len++;
        if (len > best)
            best = len;
    }
    return best;
}
