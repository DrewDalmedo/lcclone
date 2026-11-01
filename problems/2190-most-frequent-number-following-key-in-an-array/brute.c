/* Slow and obviously right: for each entry's value, scan the whole array
 * and count the positions i holding key with that value at i + 1; keep a
 * value with the largest count. */
int mostFrequent(int *nums, int numsSize, int key)
{
    int best = 0, bestCount = -1, count, i, j;

    for (j = 0; j < numsSize; j++) {
        count = 0;
        for (i = 0; i + 1 < numsSize; i++)
            if (nums[i] == key && nums[i + 1] == nums[j])
                count++;
        if (count > bestCount) {
            bestCount = count;
            best = nums[j];
        }
    }
    return best;
}
