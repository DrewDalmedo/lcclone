/* Slow and obviously right, straight from the definition: an element
 * starts a run when the number one below it is not in nums; the run then
 * goes up through every following integer that is in nums (each looked
 * up by a full scan). Numbers are turned into digits by hand, in long
 * long. */
static int has(const int *nums, int n, long long x)
{
    int i;

    for (i = 0; i < n; i++)
        if (nums[i] == x)
            return 1;
    return 0;
}

static char *put(char *p, long long v)
{
    char digits[24];
    int n = 0;

    if (v < 0) {
        *p++ = '-';
        v = -v;
    }
    do {
        digits[n++] = (char)('0' + v % 10);
        v /= 10;
    } while (v > 0);
    while (n > 0)
        *p++ = digits[--n];
    return p;
}

char **summaryRanges(int *nums, int numsSize, int *returnSize)
{
    char **out = malloc(21 * sizeof *out);
    int m = 0, i;

    for (i = 0; i < numsSize; i++) {
        long long a = nums[i], b = a;
        char *p;

        if (has(nums, numsSize, a - 1))
            continue;
        while (has(nums, numsSize, b + 1))
            b++;
        out[m] = malloc(64);
        p = put(out[m], a);
        if (b > a) {
            *p++ = '-';
            *p++ = '>';
            p = put(p, b);
        }
        *p = '\0';
        m++;
    }
    *returnSize = m;
    return out;
}
