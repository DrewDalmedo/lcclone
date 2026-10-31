/* Slow and obviously right: build the cleaned-up text, build its reverse,
 * and compare the two. */
bool isPalindrome(char *s)
{
    size_t n = strlen(s), m = 0, i;
    char *clean = malloc(n + 1), *rev = malloc(n + 1);
    bool same;

    for (i = 0; i < n; i++)
        if (isalnum((unsigned char)s[i]))
            clean[m++] = (char)tolower((unsigned char)s[i]);
    clean[m] = '\0';
    for (i = 0; i < m; i++)
        rev[i] = clean[m - 1 - i];
    rev[m] = '\0';
    same = strcmp(clean, rev) == 0;
    free(clean);
    free(rev);
    return same;
}
