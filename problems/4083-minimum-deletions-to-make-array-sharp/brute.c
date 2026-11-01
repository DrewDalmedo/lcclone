/* Slow and obviously right: try every set of elements to keep (only for
 * the short lists of the cross-check), and among those that leave a sharp
 * list take the one that deletes the fewest. */
int minDeletions(int *nums, int numsSize)
{
    unsigned long mask, full = (1ul << numsSize) - 1;
    int best = numsSize, i;

    for (mask = 0; mask <= full; mask++) {
        int kept = 0, last = 0, sharp = 1;

        for (i = 0; i < numsSize; i++) {
            if (!(mask >> i & 1ul))
                continue;
            if (kept > 0 && nums[i] == last)
                sharp = 0;
            last = nums[i];
            kept++;
        }
        if (sharp && numsSize - kept < best)
            best = numsSize - kept;
    }
    return best;
}
