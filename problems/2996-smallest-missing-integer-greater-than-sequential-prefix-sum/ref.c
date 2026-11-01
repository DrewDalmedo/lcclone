/* Reference solution: add up the sequential prefix, mark which values
 * occur in nums, then count up from the sum to the first unmarked value.
 * No value of nums is above 50, so any number above 50 is missing. */
int missingInteger(int *nums, int numsSize)
{
    bool seen[51] = {false};
    int s = nums[0], x, i;

    for (i = 1; i < numsSize && nums[i] == nums[i - 1] + 1; i++)
        s += nums[i];
    for (i = 0; i < numsSize; i++)
        seen[nums[i]] = true;
    for (x = s; x <= 50 && seen[x]; x++)
        ;
    return x;
}
