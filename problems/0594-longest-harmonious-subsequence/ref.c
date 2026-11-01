/* Reference solution: sort a copy of nums, so that equal values form
 * blocks in increasing order. A block followed by a block of values
 * exactly 1 larger gives a candidate, the two block sizes added up. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int findLHS(int *nums, int numsSize)
{
    int *a = malloc((size_t)numsSize * sizeof *a);
    int best = 0, prev_value = 0, prev_count = 0, i = 0, j;

    memcpy(a, nums, (size_t)numsSize * sizeof *a);
    qsort(a, (size_t)numsSize, sizeof *a, cmp_int);
    while (i < numsSize) {
        j = i;
        while (j < numsSize && a[j] == a[i])
            j++;
        /* prev_value <= 10^9, so prev_value + 1 can't overflow */
        if (prev_count > 0 && a[i] == prev_value + 1
            && prev_count + (j - i) > best)
            best = prev_count + (j - i);
        prev_value = a[i];
        prev_count = j - i;
        i = j;
    }
    free(a);
    return best;
}
