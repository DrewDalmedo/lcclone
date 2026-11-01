/* Reference solution: a run of equal neighbours can keep only one of its
 * elements, and keeping one per run is enough, so every element equal to
 * the one before it is a deletion. */
int minDeletions(int *nums, int numsSize)
{
    int count = 0, i;

    for (i = 1; i < numsSize; i++)
        if (nums[i] == nums[i - 1])
            count++;
    return count;
}
