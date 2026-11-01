/* Slow and obviously right: pair each letter of t with an unused equal
 * letter of s; the one letter of t that finds no partner is the extra one. */
char findTheDifference(char *s, char *t)
{
    size_t n = strlen(s), m = strlen(t), i, k;
    bool *used = calloc(n + 1, sizeof *used);
    char extra = '?';

    for (i = 0; i < m; i++) {
        for (k = 0; k < n && (used[k] || s[k] != t[i]); k++)
            ;
        if (k == n) {
            extra = t[i];
            break;
        }
        used[k] = true;
    }
    free(used);
    return extra;
}
