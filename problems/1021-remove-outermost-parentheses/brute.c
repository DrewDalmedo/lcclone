/* Slow and obviously right: cut off the blocks one at a time. The next
 * block is the shortest non-empty piece at the front of what is left that
 * is balanced by itself, each candidate tested by counting it from
 * scratch; append that piece without its first and last character. */
static int balanced(const char *s, size_t len)
{
    size_t i;
    long depth = 0;

    for (i = 0; i < len; i++) {
        depth += s[i] == '(' ? 1 : -1;
        if (depth < 0)
            return 0;
    }
    return depth == 0;
}

char *removeOuterParentheses(char *s)
{
    size_t n = strlen(s), start = 0, end, len = 0;
    char *out = malloc(n + 1);

    while (start < n) {
        end = start + 2;
        while (!balanced(s + start, end - start))
            end++;
        memcpy(out + len, s + start + 1, end - start - 2);
        len += end - start - 2;
        start = end;
    }
    out[len] = '\0';
    return out;
}
