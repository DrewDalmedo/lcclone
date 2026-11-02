/* Slow and obviously right: try every prefix length from 0 up and keep the
 * longest prefix that is empty or whose last letter is a consonant. */
static int is_vowel(char c)
{
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

char *trimTrailingVowels(char *s)
{
    int n = (int)strlen(s), best = 0, len;
    char *out;

    for (len = 1; len <= n; len++)
        if (!is_vowel(s[len - 1]))
            best = len;
    out = malloc((size_t)best + 1);
    for (len = 0; len < best; len++)
        out[len] = s[len];
    out[best] = '\0';
    return out;
}
