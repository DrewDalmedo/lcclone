/* Slow and obviously right: n / 20 times, strike out one copy of the
 * smallest element still there and one copy of the largest, then average
 * what is left. */
double trimMean(int *arr, int arrSize)
{
    int t = arrSize / 20, round, i, lo, hi;
    char *gone = calloc((size_t)arrSize, 1);
    long long sum = 0;

    for (round = 0; round < t; round++) {
        lo = hi = -1;
        for (i = 0; i < arrSize; i++)
            if (!gone[i] && (lo < 0 || arr[i] < arr[lo]))
                lo = i;
        gone[lo] = 1;
        for (i = 0; i < arrSize; i++)
            if (!gone[i] && (hi < 0 || arr[i] > arr[hi]))
                hi = i;
        gone[hi] = 1;
    }
    for (i = 0; i < arrSize; i++)
        if (!gone[i])
            sum += arr[i];
    free(gone);
    return (double)sum / (arrSize - 2 * t);
}
