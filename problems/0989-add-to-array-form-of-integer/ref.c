/* Reference solution: add k into the digits from the units up, keeping
 * all of what is left of k as the carry (num[i] + k stays below 10^4 + 10).
 * The sum has at most max(n, 5) + 1 digits; it is built from the back of
 * a buffer that size and then moved to the front. */
int *addToArrayForm(int *num, int numSize, int k, int *returnSize)
{
    int cap = (numSize > 5 ? numSize : 5) + 1;
    int *out = malloc((size_t)cap * sizeof *out);
    int pos = cap, i = numSize - 1, carry = k, len;

    while (i >= 0 || carry > 0) {
        int cur = carry;

        if (i >= 0)
            cur += num[i--];
        out[--pos] = cur % 10;
        carry = cur / 10;
    }
    len = cap - pos;
    memmove(out, out + pos, (size_t)len * sizeof *out);
    *returnSize = len;
    return out;
}
