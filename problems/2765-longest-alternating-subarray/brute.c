/* Slow and obviously right: for every stretch nums[l..r] of two or more
 * elements, check straight from the definition that its elements are
 * nums[l] at even offsets and nums[l] + 1 at odd ones. */
int alternatingSubarray(int *nums, int numsSize)
{
    int best = -1, l, r, i, ok;

    for (l = 0; l < numsSize; l++)
        for (r = l + 1; r < numsSize; r++) {
            ok = 1;
            for (i = l; ok && i <= r; i++)
                if (nums[i] != nums[l] + (i - l) % 2)
                    ok = 0;
            if (ok && r - l + 1 > best)
                best = r - l + 1;
        }
    return best;
}
