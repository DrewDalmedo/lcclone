/* Slow and obviously right: for each entry, count the entries equal to it
 * by scanning the whole array, and add the entry when that count is a
 * multiple of k. */
int sumDivisibleByK(int *nums, int numsSize, int k)
{
    int total = 0, count, i, j;

    for (i = 0; i < numsSize; i++) {
        count = 0;
        for (j = 0; j < numsSize; j++)
            if (nums[j] == nums[i])
                count++;
        if (count % k == 0)
            total += nums[i];
    }
    return total;
}
