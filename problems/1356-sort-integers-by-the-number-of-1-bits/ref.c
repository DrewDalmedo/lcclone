/* Reference solution: sort a copy with qsort by one key, the bit count
 * times 2^14 plus the value. Every value is below 2^14, so the key orders
 * by bit count first and by value within a bit count, and since equal keys
 * mean equal values, qsort's lack of stability does no harm. */
static int bits_key(int v)
{
    int b = 0, x = v;

    while (x) {
        x &= x - 1;
        b++;
    }
    return (b << 14) | v;
}

static int by_key(const void *a, const void *b)
{
    int ka = bits_key(*(const int *)a), kb = bits_key(*(const int *)b);

    return (ka > kb) - (ka < kb);
}

int *sortByBits(int *arr, int arrSize, int *returnSize)
{
    int *out = malloc((size_t)arrSize * sizeof *out);

    memcpy(out, arr, (size_t)arrSize * sizeof *out);
    qsort(out, (size_t)arrSize, sizeof *out, by_key);
    *returnSize = arrSize;
    return out;
}
