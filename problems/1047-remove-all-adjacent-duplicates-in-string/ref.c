/* Reference solution: scan s from left to right, keeping the letters that
 * survive so far on a stack. A letter equal to the top of the stack cuts
 * it out; any other letter is pushed. The stack, bottom to top, is what
 * remains. */
char *removeDuplicates(char *s)
{
    size_t n = strlen(s), top = 0, i;
    char *out = malloc(n + 1);

    for (i = 0; i < n; i++) {
        if (top > 0 && out[top - 1] == s[i])
            top--;
        else
            out[top++] = s[i];
    }
    out[top] = '\0';
    return out;
}
