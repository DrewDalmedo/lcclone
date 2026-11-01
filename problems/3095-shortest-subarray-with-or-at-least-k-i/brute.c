/* Slow and obviously right: OR together the elements of every slice
 * anew, and keep the shortest one that reaches k. */
int minimumSubarrayLength(int *nums, int numsSize, int k)
{
    int best = -1, i, j, t, value;

    for (i = 0; i < numsSize; i++)
        for (j = i; j < numsSize; j++) {
            value = 0;
            for (t = i; t <= j; t++)
                value |= nums[t];
            if (value >= k && (best == -1 || j - i + 1 < best))
                best = j - i + 1;
        }
    return best;
}
