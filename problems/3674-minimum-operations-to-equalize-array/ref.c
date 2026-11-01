/* Reference solution: if the entries are already all equal, no squash is
 * needed. Otherwise one is: squashing the whole row writes the AND of all
 * entries everywhere. */
int minOperations(int *nums, int numsSize)
{
    int i;

    for (i = 1; i < numsSize; i++)
        if (nums[i] != nums[0])
            return 1;
    return 0;
}
