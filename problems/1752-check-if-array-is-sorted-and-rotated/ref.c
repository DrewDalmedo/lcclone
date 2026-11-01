/* Reference solution: walk once around the array as if it were a ring,
 * comparing each value with the next one and the last with the first.
 * The array is a rotated sorted one exactly when the values go down at
 * most once on the way round. */
bool check(int *nums, int numsSize)
{
    int drops = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] > nums[(i + 1) % numsSize])
            drops++;
    return drops <= 1;
}
