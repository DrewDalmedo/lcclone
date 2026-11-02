/* Slow and obviously right: write n out as a string and try every pair of
 * positions. */
int maxProduct(int n)
{
    char s[16];
    int len = snprintf(s, sizeof s, "%d", n), best = 0, i, j, p;

    for (i = 0; i < len; i++)
        for (j = i + 1; j < len; j++) {
            p = (s[i] - '0') * (s[j] - '0');
            if (p > best)
                best = p;
        }
    return best;
}
