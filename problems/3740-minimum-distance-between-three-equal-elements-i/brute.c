/* Slow and obviously right: try every three positions holding the same
 * value and add up their pairwise distances. */
int minimumDistance(int *nums, int numsSize)
{
    int best = -1, i, j, k, spread;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++) {
                if (nums[i] != nums[j] || nums[j] != nums[k])
                    continue;
                spread = abs(i - j) + abs(j - k) + abs(i - k);
                if (best < 0 || spread < best)
                    best = spread;
            }
    return best;
}
