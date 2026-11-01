/* Slow and obviously right: sort a copy of nums to read off the smallest
 * and largest values, then try every candidate d from the smallest value
 * down to 1 and return the first that divides both. */
static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int findGCD(int *nums, int numsSize)
{
    int *s = malloc((size_t)numsSize * sizeof *s);
    int lo, hi, d;

    memcpy(s, nums, (size_t)numsSize * sizeof *s);
    qsort(s, (size_t)numsSize, sizeof *s, cmp_int);
    lo = s[0];
    hi = s[numsSize - 1];
    free(s);
    for (d = lo; d > 1; d--)
        if (lo % d == 0 && hi % d == 0)
            break;
    return d;
}
