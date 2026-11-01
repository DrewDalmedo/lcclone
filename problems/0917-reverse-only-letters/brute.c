/* Slow and obviously right, straight from the definition: the k-th letter
 * from the left of the result is the k-th letter from the right of s. For
 * each letter of s, count the letters before it, then scan s from the right
 * for the letter with that many letters after it. */
static int is_letter(char c)
{
    return c != '\0' && strchr("abcdefghijklmnopqrstuvwxyz"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ", c) != NULL;
}

char *reverseOnlyLetters(char *s)
{
    int n = (int)strlen(s), i, j, k, seen;
    char *r = malloc((size_t)n + 1);

    for (i = 0; i < n; i++) {
        r[i] = s[i];
        if (!is_letter(s[i]))
            continue;
        k = 0;
        for (j = 0; j < i; j++)
            if (is_letter(s[j]))
                k++;
        seen = 0;
        for (j = n - 1; j >= 0; j--)
            if (is_letter(s[j]) && seen++ == k) {
                r[i] = s[j];
                break;
            }
    }
    r[n] = '\0';
    return r;
}
