/* Slow and obviously right: for each position, count how many characters
 * of its kind stand before it (its rank k from the left), then find the
 * character of that kind with rank k from the right and put it there. */
static int kind(char c)
{
    return c >= 'a' && c <= 'z';
}

char *reverseByType(char *s)
{
    int n = (int)strlen(s), i, j, k, seen;
    char *t = malloc((size_t)n + 1);

    for (i = 0; i < n; i++) {
        k = 0;
        for (j = 0; j < i; j++)
            if (kind(s[j]) == kind(s[i]))
                k++;
        seen = 0;
        for (j = n - 1; j >= 0; j--)
            if (kind(s[j]) == kind(s[i]) && seen++ == k)
                break;
        t[i] = s[j];
    }
    t[n] = '\0';
    return t;
}
