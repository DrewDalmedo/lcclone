/* Slow and obviously right: try every piece num[i..j] whose last digit is
 * odd and keep the largest. Two pieces are compared as numbers: drop their
 * leading zeros, then the one with more digits is larger, and pieces of
 * equal length compare digit by digit. */
static int compare_pieces(const char *a, int alen, const char *b, int blen)
{
    while (alen > 1 && *a == '0') {
        a++;
        alen--;
    }
    while (blen > 1 && *b == '0') {
        b++;
        blen--;
    }
    if (alen != blen)
        return alen < blen ? -1 : 1;
    return memcmp(a, b, (size_t)alen);
}

char *largestOddNumber(char *num)
{
    int n = (int)strlen(num), i, j, best = 0, bestlen = 0;
    char *out;

    for (i = 0; i < n; i++)
        for (j = i; j < n; j++)
            if ((num[j] - '0') % 2 == 1 &&
                (bestlen == 0 ||
                 compare_pieces(num + i, j - i + 1, num + best, bestlen) > 0)) {
                best = i;
                bestlen = j - i + 1;
            }
    out = malloc((size_t)bestlen + 1);
    memcpy(out, num + best, (size_t)bestlen);
    out[bestlen] = '\0';
    return out;
}
