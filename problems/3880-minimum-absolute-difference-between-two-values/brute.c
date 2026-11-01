/* Slow and obviously right: try every pair of positions holding a 1 and
 * a 2, and keep the smallest distance. */
int minAbsoluteDifference(int *nums, int numsSize)
{
    int best = -1, i, j, d;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++)
            if (nums[i] == 1 && nums[j] == 2) {
                d = i > j ? i - j : j - i;
                if (best < 0 || d < best)
                    best = d;
            }
    return best;
}
