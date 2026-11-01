/* Slow and obviously right: for every even entry, count its copies by
 * going through the whole array again, and keep the value with the most
 * copies, the smaller value on a tie. */
int mostFrequentEven(int *nums, int numsSize)
{
    int best = -1, bestCount = 0, count, i, k;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] % 2 != 0)
            continue;
        count = 0;
        for (k = 0; k < numsSize; k++)
            if (nums[k] == nums[i])
                count++;
        if (count > bestCount || (count == bestCount && nums[i] < best)) {
            best = nums[i];
            bestCount = count;
        }
    }
    return best;
}
