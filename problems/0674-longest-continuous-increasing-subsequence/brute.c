/* Slow and obviously right: check every block nums[l..r], pair of
 * neighbours by pair of neighbours, and keep the length of the longest
 * block that climbs. */
int findLengthOfLCIS(int *nums, int numsSize)
{
    int best = 0, climbs, l, r, i;

    for (l = 0; l < numsSize; l++)
        for (r = l; r < numsSize; r++) {
            climbs = 1;
            for (i = l; i < r; i++)
                if (nums[i] >= nums[i + 1])
                    climbs = 0;
            if (climbs && r - l + 1 > best)
                best = r - l + 1;
        }
    return best;
}
