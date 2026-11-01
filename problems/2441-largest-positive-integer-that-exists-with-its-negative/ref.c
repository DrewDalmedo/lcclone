/* Reference solution: one pass with a table of the values seen so far,
 * indexed by value + 1000. When a value's opposite is already in the
 * table, its absolute value is mirrored; keep the largest. */
int findMaxK(int *nums, int numsSize)
{
    char seen[2001] = {0};
    int best = -1, i, x;

    for (i = 0; i < numsSize; i++) {
        x = nums[i];
        seen[x + 1000] = 1;
        if (seen[-x + 1000]) {
            if (x < 0)
                x = -x;
            if (x > best)
                best = x;
        }
    }
    return best;
}
