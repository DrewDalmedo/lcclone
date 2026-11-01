/* Reference solution: one pass with the depth counter. A '(' is the first
 * character of a block when the depth before it is 0, and a ')' is the
 * last when the depth after it is 0; copy every other character. */
char *removeOuterParentheses(char *s)
{
    size_t n = strlen(s), i, len = 0;
    char *out = malloc(n + 1);
    int depth = 0;

    for (i = 0; i < n; i++) {
        if (s[i] == '(') {
            if (depth > 0)
                out[len++] = s[i];
            depth++;
        } else {
            depth--;
            if (depth > 0)
                out[len++] = s[i];
        }
    }
    out[len] = '\0';
    return out;
}
