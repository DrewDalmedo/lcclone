/* Reference solution: one pass from left to right, copying each letter of
 * s to the answer unless the last two letters copied are both that letter
 * already. Nothing may be struck, so the answer needs room for all of s
 * and its NUL. */
char *makeFancyString(char *s)
{
    size_t n = strlen(s), len = 0, i;
    char *out = malloc(n + 1);

    for (i = 0; i < n; i++) {
        if (len >= 2 && out[len - 1] == s[i] && out[len - 2] == s[i])
            continue;
        out[len++] = s[i];
    }
    out[len] = '\0';
    return out;
}
