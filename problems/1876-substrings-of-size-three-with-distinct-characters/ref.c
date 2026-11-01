/* Reference solution: look at each run of three characters in turn and
 * count those whose letters differ pairwise. The loop tests i + 2 < n with
 * n an int, so a string shorter than three has no run to look at. */
int countGoodSubstrings(char *s)
{
    int n = (int)strlen(s), count = 0, i;

    for (i = 0; i + 2 < n; i++)
        if (s[i] != s[i + 1] && s[i + 1] != s[i + 2] && s[i] != s[i + 2])
            count++;
    return count;
}
