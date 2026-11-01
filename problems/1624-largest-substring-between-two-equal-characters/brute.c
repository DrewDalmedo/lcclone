/* Slow and obviously right: try every pair of positions i < j holding the
 * same letter and keep the longest stretch between them. */
int maxLengthBetweenEqualCharacters(char *s)
{
    int n = (int)strlen(s), best = -1, i, j;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (s[i] == s[j] && j - i - 1 > best)
                best = j - i - 1;
    return best;
}
