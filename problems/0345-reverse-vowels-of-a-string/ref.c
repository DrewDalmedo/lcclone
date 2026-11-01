/* Reference solution: copy s, then move one index in from each end of the
 * copy, each stopping at the next vowel, and swap the two vowels until the
 * indices meet. */
static bool is_vowel(char c)
{
    return c != '\0' && strchr("aeiouAEIOU", c) != NULL;
}

char *reverseVowels(char *s)
{
    size_t n = strlen(s), i = 0, j = n;
    char *r = malloc(n + 1), t;

    memcpy(r, s, n + 1);
    for (;;) {
        while (i < j && !is_vowel(r[i]))
            i++;
        while (i < j && !is_vowel(r[j - 1]))
            j--;
        if (j - i < 2)
            break;
        t = r[i];
        r[i++] = r[j - 1];
        r[--j] = t;
    }
    return r;
}
