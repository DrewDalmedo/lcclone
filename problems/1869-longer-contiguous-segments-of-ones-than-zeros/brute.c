/* Slow and obviously right: try every substring, and when all its
 * characters are equal, let its length count toward that character's
 * best. */
bool checkZeroOnes(char *s)
{
    int n = (int)strlen(s), best[2] = {0, 0}, i, j, k, same;

    for (i = 0; i < n; i++)
        for (j = i; j < n; j++) {
            same = 1;
            for (k = i; k <= j; k++)
                if (s[k] != s[i])
                    same = 0;
            if (same && j - i + 1 > best[s[i] - '0'])
                best[s[i] - '0'] = j - i + 1;
        }
    return best[1] > best[0];
}
