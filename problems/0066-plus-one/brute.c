/* Slow and obviously right: count up from zero. Keep a counter as decimal
 * digits, least significant first, and increment it by one, digit by
 * digit with carries, until it equals digits read backwards; one more
 * increment gives the answer. Only quick for small numbers, which is all
 * the cross-check uses. */
static void increment(int *c, int *len)
{
    int i = 0;

    while (i < *len && c[i] == 9)
        c[i++] = 0;
    if (i == *len)
        c[(*len)++] = 1;
    else
        c[i]++;
}

static int equal(const int *c, int len, const int *digits, int digitsSize)
{
    int i;

    if (len != digitsSize)
        return 0;
    for (i = 0; i < len; i++)
        if (c[i] != digits[digitsSize - 1 - i])
            return 0;
    return 1;
}

int *plusOne(int *digits, int digitsSize, int *returnSize)
{
    int *c = calloc((size_t)digitsSize + 2, sizeof *c);
    int *out;
    int len = 1, i;

    while (!equal(c, len, digits, digitsSize))
        increment(c, &len);
    increment(c, &len);
    out = malloc((size_t)len * sizeof *out);
    for (i = 0; i < len; i++)
        out[i] = c[len - 1 - i];
    free(c);
    *returnSize = len;
    return out;
}
