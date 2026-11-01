/* Reference solution: add the bits column by column from the right end of
 * both strings, with a carry, filling the answer from its right end. The
 * answer has room for one more bit than the longer string; when there is
 * no carry out of the top column, that spare first slot is dropped. */
char *addBinary(char *a, char *b)
{
    size_t la = strlen(a), lb = strlen(b);
    size_t n = (la > lb ? la : lb) + 1, k = n;
    char *out = malloc(n + 1);
    int carry = 0, sum;

    out[n] = '\0';
    while (la > 0 || lb > 0) {
        sum = carry;
        if (la > 0)
            sum += a[--la] - '0';
        if (lb > 0)
            sum += b[--lb] - '0';
        out[--k] = (char)('0' + (sum & 1));
        carry = sum >> 1;
    }
    if (carry) {
        out[0] = '1';
        return out;
    }
    memmove(out, out + 1, n);
    return out;
}
