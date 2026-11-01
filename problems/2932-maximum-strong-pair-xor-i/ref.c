/* Reference solution: sort a copy of nums. For each value x, its partners
 * y >= x in a strong pair are the values from x up to 2 * x, a run that
 * starts at x's own position, so scan that run and stop at the first value
 * past 2 * x. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int maximumStrongPairXor(int *nums, int numsSize)
{
    int *v = malloc((size_t)numsSize * sizeof *v);
    int best = 0, i, j;

    memcpy(v, nums, (size_t)numsSize * sizeof *v);
    qsort(v, (size_t)numsSize, sizeof *v, cmp_int);
    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize && v[j] <= 2 * v[i]; j++)
            if ((v[i] ^ v[j]) > best)
                best = v[i] ^ v[j];
    free(v);
    return best;
}
