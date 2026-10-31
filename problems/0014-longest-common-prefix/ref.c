/* Reference solution: compare every word with the first, column by
 * column, until a word ends or a letter differs. */
char *longestCommonPrefix(char **strs, int strsSize)
{
    size_t len = strlen(strs[0]), i;
    int k;
    char *out;

    for (k = 1; k < strsSize; k++) {
        for (i = 0; i < len && strs[k][i] == strs[0][i]; i++)
            ;
        len = i;
    }
    out = malloc(len + 1);
    memcpy(out, strs[0], len);
    out[len] = '\0';
    return out;
}
