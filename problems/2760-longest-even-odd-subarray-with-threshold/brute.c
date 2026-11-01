/* Slow and obviously right: test every block nums[l..r] against the three
 * rules one by one, and keep the length of the longest that passes. */
int longestAlternatingSubarray(int *nums, int numsSize, int threshold)
{
    int best = 0, l, r, i, ok;

    for (l = 0; l < numsSize; l++)
        for (r = l; r < numsSize; r++) {
            ok = nums[l] % 2 == 0;
            for (i = l; ok && i < r; i++)
                if (nums[i] % 2 == nums[i + 1] % 2)
                    ok = 0;
            for (i = l; ok && i <= r; i++)
                if (nums[i] > threshold)
                    ok = 0;
            if (ok && r - l + 1 > best)
                best = r - l + 1;
        }
    return best;
}
