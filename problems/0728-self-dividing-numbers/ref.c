/* Reference solution: test each number of the range digit by digit. The
 * digits are peeled off a copy, so that it is always the number itself
 * that gets divided, and a 0 digit rules the number out before any
 * division by it. */
static bool self_dividing(int x)
{
    int rest, d;

    for (rest = x; rest > 0; rest /= 10) {
        d = rest % 10;
        if (d == 0 || x % d != 0)
            return false;
    }
    return true;
}

int *selfDividingNumbers(int left, int right, int *returnSize)
{
    int *out = malloc((size_t)(right - left + 1) * sizeof *out);
    int n = 0, x;

    for (x = left; x <= right; x++)
        if (self_dividing(x))
            out[n++] = x;
    *returnSize = n;
    return out;
}
