/* Right, but too slow at full size: for the k-th vowel from the left,
 * find its partner, the k-th vowel from the right, by scanning back from
 * the end of s again each time. That is O(n) per vowel, O(n^2) in all. */
static int vowel(char c)
{
    return strchr("aeiouAEIOU", c) != NULL && c != '\0';
}

char *reverseVowels(char *s)
{
    int n = (int)strlen(s), i, j, k = 0, seen;
    char *out = malloc((size_t)n + 1);

    memcpy(out, s, (size_t)n + 1);
    for (i = 0; i < n; i++) {
        if (!vowel(s[i]))
            continue;
        seen = 0;
        for (j = n - 1; j >= 0; j--)
            if (vowel(s[j]) && seen++ == k)
                break;
        out[i] = s[j];
        k++;
    }
    return out;
}
