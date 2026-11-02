/* Slow and obviously right: print n as text, and for each non-zero digit
 * build the text of that digit followed by one '0' per digit to its right,
 * then read it back as a number. */
int *decimalRepresentation(int n, int *returnSize)
{
    char digits[16], term[16];
    int *out = malloc(16 * sizeof *out);
    int len, i, k, m = 0;

    len = snprintf(digits, sizeof digits, "%d", n);
    for (i = 0; i < len; i++) {
        if (digits[i] == '0')
            continue;
        term[0] = digits[i];
        for (k = 1; k < len - i; k++)
            term[k] = '0';
        term[len - i] = '\0';
        out[m++] = (int)strtol(term, NULL, 10);
    }
    *returnSize = m;
    return out;
}
