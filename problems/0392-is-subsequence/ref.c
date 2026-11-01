/* Reference solution: walk through t once, matching the next letter of s
 * whenever t shows it. Taking the earliest possible match for each letter
 * never hurts, since it leaves the most of t for the letters after it. */
bool isSubsequence(char *s, char *t)
{
    size_t i = 0, j;

    for (j = 0; s[i] && t[j]; j++)
        if (t[j] == s[i])
            i++;
    return s[i] == '\0';
}
