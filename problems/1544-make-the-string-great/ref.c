/* Reference solution: one pass from left to right, keeping the characters
 * that survive so far as a stack. A character that clashes with the top of
 * the stack erases it; any other character is pushed. */
char *makeGood(char *s)
{
    size_t len = strlen(s), i, top = 0;
    char *out = malloc(len + 1);

    for (i = 0; i < len; i++) {
        if (top > 0 && out[top - 1] != s[i]
            && tolower((unsigned char)out[top - 1]) == tolower((unsigned char)s[i]))
            top--;
        else
            out[top++] = s[i];
    }
    out[top] = '\0';
    return out;
}
