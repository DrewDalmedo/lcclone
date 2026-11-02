/* Slow and obviously right: k times, take out the largest element still
 * there and add it to one sum; then, from a fresh copy, take out the
 * smallest element k times. Return the absolute difference. */
int absDifference(int *nums, int numsSize, int k)
{
    int *used = malloc((size_t)numsSize * sizeof *used);
    int big = 0, small = 0, round, best, i, diff;

    for (i = 0; i < numsSize; i++)
        used[i] = 0;
    for (round = 0; round < k; round++) {
        best = -1;
        for (i = 0; i < numsSize; i++)
            if (!used[i] && (best < 0 || nums[i] > nums[best]))
                best = i;
        used[best] = 1;
        big += nums[best];
    }
    for (i = 0; i < numsSize; i++)
        used[i] = 0;
    for (round = 0; round < k; round++) {
        best = -1;
        for (i = 0; i < numsSize; i++)
            if (!used[i] && (best < 0 || nums[i] < nums[best]))
                best = i;
        used[best] = 1;
        small += nums[best];
    }
    free(used);
    diff = big - small;
    return diff < 0 ? -diff : diff;
}
