/* Reference solution: count every value in a table indexed by value, then
 * scan nums from the front for an even value whose count is 1. */
int firstUniqueEven(int *nums, int numsSize)
{
    int count[101] = {0};
    int i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (i = 0; i < numsSize; i++)
        if (nums[i] % 2 == 0 && count[nums[i]] == 1)
            return nums[i];
    return -1;
}
