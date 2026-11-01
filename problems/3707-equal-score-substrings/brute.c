/* Slow and obviously right: try every split, scoring both substrings from
 * scratch by looking up each letter's place in the alphabet. */
static int score(const char *s, int from, int to)
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int total = 0, i;

    for (i = from; i < to; i++)
        total += (int)(strchr(alphabet, s[i]) - alphabet) + 1;
    return total;
}

bool scoreBalance(char *s)
{
    int n = (int)strlen(s), i;

    for (i = 1; i < n; i++)
        if (score(s, 0, i) == score(s, i, n))
            return true;
    return false;
}
