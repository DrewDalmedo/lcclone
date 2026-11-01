/* Slow and obviously right: split b into powers of two, one for each of
 * its 1 bits, and add them to a one at a time. Adding 2^p is incrementing
 * from bit p: 1s turn into 0s until the first 0, which turns into a 1. The
 * bits are kept least significant first; at the end they are written out
 * most significant first, from the highest 1 (or "0" if there is none). */
char *addBinary(char *a, char *b)
{
    int la = (int)strlen(a), lb = (int)strlen(b);
    int n = (la > lb ? la : lb) + 1, top, i, p;
    char *bits = calloc((size_t)n, 1);
    char *out;

    for (i = 0; i < la; i++)
        bits[i] = (char)(a[la - 1 - i] - '0');
    for (p = 0; p < lb; p++) {
        if (b[lb - 1 - p] != '1')
            continue;
        for (i = p; bits[i] == 1; i++)
            bits[i] = 0;
        bits[i] = 1;
    }
    for (top = n - 1; top > 0 && bits[top] == 0; top--)
        ;
    out = malloc((size_t)top + 2);
    for (i = 0; i <= top; i++)
        out[i] = (char)('0' + bits[top - i]);
    out[top + 1] = '\0';
    free(bits);
    return out;
}
