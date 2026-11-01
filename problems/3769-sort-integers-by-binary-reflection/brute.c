/* Slow and obviously right: write each number out as a string of 0s and
 * 1s, reverse the string, read it back in base 2, and insertion-sort by
 * (reflection, value). */
static long long reflection(int x)
{
    char bits[40];
    int len = 0, i;
    long long v = 0;

    while (x > 0) {
        bits[len++] = (char)('0' + x % 2);
        x /= 2;
    }
    /* bits[] holds the lowest bit first, so the binary form of x is bits
     * read backwards; its reversal is bits read forwards. */
    for (i = 0; i < len; i++)
        v = v * 2 + (bits[i] - '0');
    return v;
}

static int before(int a, int b)
{
    long long ra = reflection(a), rb = reflection(b);

    return ra < rb || (ra == rb && a < b);
}

int *sortByReflection(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, j, v;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        for (j = i; j > 0 && before(v, out[j - 1]); j--)
            out[j] = out[j - 1];
        out[j] = v;
    }
    *returnSize = numsSize;
    return out;
}
