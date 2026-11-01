/* Slow and obviously right: count how often each value occurs, then for
 * each bit count from 0 to 13, write out every value from 0 to 10^4 that
 * has that bit count, in increasing order, as often as it occurs. */
static int bit_count(int v)
{
    int b = 0;

    while (v > 0) {
        b += v % 2;
        v /= 2;
    }
    return b;
}

int *sortByBits(int *arr, int arrSize, int *returnSize)
{
    static int seen[10001];
    int *out = malloc((size_t)arrSize * sizeof *out);
    int k = 0, b, v, i;

    memset(seen, 0, sizeof seen);
    for (i = 0; i < arrSize; i++)
        seen[arr[i]]++;
    for (b = 0; b <= 13; b++)
        for (v = 0; v <= 10000; v++)
            if (bit_count(v) == b)
                for (i = 0; i < seen[v]; i++)
                    out[k++] = v;
    *returnSize = k;
    return out;
}
