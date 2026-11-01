/* Slow and obviously right: write each number out as text, rule it out if
 * the text has a '0', and otherwise check that the number is a multiple
 * of every digit 1 to 9 that appears in the text. */
int *selfDividingNumbers(int left, int right, int *returnSize)
{
    int *out = malloc((size_t)(right - left + 1) * sizeof *out);
    char text[16];
    int n = 0, x, d, ok;

    for (x = left; x <= right; x++) {
        sprintf(text, "%d", x);
        ok = strchr(text, '0') == NULL;
        for (d = 1; d <= 9 && ok; d++)
            if (strchr(text, '0' + d) && x % d != 0)
                ok = 0;
        if (ok)
            out[n++] = x;
    }
    *returnSize = n;
    return out;
}
