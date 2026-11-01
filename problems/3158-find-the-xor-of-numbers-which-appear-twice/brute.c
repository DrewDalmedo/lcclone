/* Slow and obviously right: look at every pair of positions i < j. A
 * doubled value has exactly one pair of equal elements, so XOR it in
 * there; a value that occurs once has none. */
int duplicateNumbersXOR(int *nums, int numsSize)
{
    int result = 0, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] == nums[j])
                result ^= nums[i];
    return result;
}
