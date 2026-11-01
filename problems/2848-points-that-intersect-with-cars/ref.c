/* Reference solution: sort the cars by their first marker and sweep them
 * in that order, keeping the span merged so far. A car that starts at or
 * before the end of that span extends it (with max, since it may end
 * earlier); any other car closes the span, whose markers are counted, and
 * starts a new one. */
static int by_start(const void *a, const void *b)
{
    const int *x = a, *y = b;

    if (x[0] != y[0])
        return x[0] < y[0] ? -1 : 1;
    return (x[1] > y[1]) - (x[1] < y[1]);
}

int numberOfPoints(int **nums, int numsSize, int *numsColSize)
{
    int *cars = malloc((size_t)numsSize * 2 * sizeof *cars);
    int total = 0, lo, hi, i;

    (void)numsColSize;
    for (i = 0; i < numsSize; i++) {
        cars[2 * i] = nums[i][0];
        cars[2 * i + 1] = nums[i][1];
    }
    qsort(cars, (size_t)numsSize, 2 * sizeof *cars, by_start);
    lo = cars[0];
    hi = cars[1];
    for (i = 1; i < numsSize; i++) {
        if (cars[2 * i] <= hi) {
            if (cars[2 * i + 1] > hi)
                hi = cars[2 * i + 1];
        } else {
            total += hi - lo + 1;
            lo = cars[2 * i];
            hi = cars[2 * i + 1];
        }
    }
    total += hi - lo + 1;
    free(cars);
    return total;
}
