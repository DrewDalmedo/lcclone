/* Reference solution: copy s, then reverse each kind in its own places
 * with two pointers: move i right and j left until both stand on a
 * character of the kind, swap, and repeat until they meet. */
static int is_letter(char c)
{
    return c >= 'a' && c <= 'z';
}

static void reverse_kind(char *t, int n, int letters)
{
    int i = 0, j = n - 1;
    char tmp;

    for (;;) {
        while (i < j && is_letter(t[i]) != letters)
            i++;
        while (i < j && is_letter(t[j]) != letters)
            j--;
        if (i >= j)
            return;
        tmp = t[i];
        t[i++] = t[j];
        t[j--] = tmp;
    }
}

char *reverseByType(char *s)
{
    int n = (int)strlen(s);
    char *t = malloc((size_t)n + 1);

    memcpy(t, s, (size_t)n + 1);
    reverse_kind(t, n, 1);
    reverse_kind(t, n, 0);
    return t;
}
