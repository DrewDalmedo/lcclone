/* Slow and obviously right: try every four indices a < b < c < d and test
 * the definition. */
int countQuadruplets(int *nums, int numsSize)
{
    int count = 0, a, b, c, d;

    for (a = 0; a < numsSize; a++)
        for (b = a + 1; b < numsSize; b++)
            for (c = b + 1; c < numsSize; c++)
                for (d = c + 1; d < numsSize; d++)
                    if (nums[a] + nums[b] + nums[c] == nums[d])
                        count++;
    return count;
}
