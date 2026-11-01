/* Slow and obviously right: print n with %d, then copy the digits from the
 * left, adding a '.' after each digit that has a non-zero multiple of 3
 * digits after it. */
char *thousandSeparator(int n)
{
    char digits[16];
    char *out = malloc(32);
    int len, i, k = 0;

    snprintf(digits, sizeof digits, "%d", n);
    len = (int)strlen(digits);
    for (i = 0; i < len; i++) {
        out[k++] = digits[i];
        if ((len - 1 - i) > 0 && (len - 1 - i) % 3 == 0)
            out[k++] = '.';
    }
    out[k] = '\0';
    return out;
}
