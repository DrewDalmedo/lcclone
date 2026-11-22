/* Slow and obviously right: try each prefix of the first word, longest
 * first, until every word starts with it. */
char *longestCommonPrefix(char **strs, int strsSize)
{
    size_t len = strlen(strs[0]);
    char *out;
    int k;

    for (;; len--) {
        for (k = 0; k < strsSize; k++)
            if (strlen(strs[k]) < len || strncmp(strs[k], strs[0], len) != 0)
                break;
        if (k == strsSize)
            break;
    }
    out = malloc(len + 1);
    memcpy(out, strs[0], len);
    out[len] = '\0';
    return out;
}
