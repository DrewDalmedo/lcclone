/* Slow and obviously right: for every index, scan the whole list for the
 * elements one step away, count how many are lower and how many higher,
 * and keep separate sums for peaks and valleys, subtracted at the end. */
int peakValleyDifference(int *nums, int numsSize)
{
    int peaks = 0, valleys = 0, i, j;

    for (i = 0; i < numsSize; i++) {
        int near = 0, lower = 0, higher = 0;

        for (j = 0; j < numsSize; j++) {
            if (j != i - 1 && j != i + 1)
                continue;
            near++;
            if (nums[j] < nums[i])
                lower++;
            if (nums[j] > nums[i])
                higher++;
        }
        if (near == 2 && lower == 2)
            peaks += nums[i];
        if (near == 2 && higher == 2)
            valleys += nums[i];
    }
    return peaks - valleys;
}
