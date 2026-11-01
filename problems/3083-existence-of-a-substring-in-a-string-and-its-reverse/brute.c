/* Slow and obviously right: spell s backwards, then compare every link of
 * s with every link of the backwards string. */
bool isSubstringPresent(char *s)
{
    size_t n = strlen(s), i, j;
    char *r = malloc(n + 1);
    bool found = false;

    for (i = 0; i < n; i++)
        r[i] = s[n - 1 - i];
    r[n] = '\0';
    for (i = 0; i + 1 < n; i++)
        for (j = 0; j + 1 < n; j++)
            if (s[i] == r[j] && s[i + 1] == r[j + 1])
                found = true;
    free(r);
    return found;
}
