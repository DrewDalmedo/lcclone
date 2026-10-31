/* Slow and obviously right: do the rounds as described, finding the
 * smallest and largest remaining elements by scanning and crossing them
 * off. */
double minimumAverage(int *nums, int numsSize)
{
    int *gone = calloc((size_t)numsSize, sizeof *gone);
    double best = 0, avg;
    int round, i, lo, hi;

    for (round = 0; round < numsSize / 2; round++) {
        lo = -1;
        for (i = 0; i < numsSize; i++)
            if (!gone[i] && (lo < 0 || nums[i] < nums[lo]))
                lo = i;
        gone[lo] = 1;
        hi = -1;
        for (i = 0; i < numsSize; i++)
            if (!gone[i] && (hi < 0 || nums[i] > nums[hi]))
                hi = i;
        gone[hi] = 1;
        avg = (nums[lo] + nums[hi]) / 2.0;
        if (round == 0 || avg < best)
            best = avg;
    }
    free(gone);
    return best;
}
