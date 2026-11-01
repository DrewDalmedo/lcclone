/* Slow and obviously right: try every three positions i < j < k and
 * count those whose values differ pairwise. */
int unequalTriplets(int *nums, int numsSize)
{
    int total = 0, i, j, k;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++)
                if (nums[i] != nums[j] && nums[i] != nums[k] &&
                    nums[j] != nums[k])
                    total++;
    return total;
}
