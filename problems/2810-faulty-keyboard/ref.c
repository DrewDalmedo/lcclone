/* Reference solution: rather than turning the line around, keep a flag
 * saying whether it is stored back to front in a buffer that can grow at
 * both ends. A letter goes on the right end while the flag is clear and on
 * the left end while it is set; each 'i' flips the flag. At the end the
 * buffer is read out forwards, or backwards if the flag is set. */
char *finalString(char *s)
{
    int n = (int)strlen(s), lo = n, hi = n, flipped = 0, i, k;
    char *buf = malloc(2 * (size_t)n + 1);
    char *out;

    for (i = 0; i < n; i++) {
        if (s[i] == 'i')
            flipped = !flipped;
        else if (flipped)
            buf[--lo] = s[i];
        else
            buf[hi++] = s[i];
    }
    out = malloc((size_t)(hi - lo) + 1);
    for (k = 0; k < hi - lo; k++)
        out[k] = flipped ? buf[hi - 1 - k] : buf[lo + k];
    out[hi - lo] = '\0';
    free(buf);
    return out;
}
