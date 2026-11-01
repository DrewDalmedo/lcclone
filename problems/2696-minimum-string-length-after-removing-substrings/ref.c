/* Reference solution: scan s with a stack of the letters kept so far. A
 * B on top of an A, or a D on top of a C, deletes that pair at once (which
 * may expose an earlier letter to the next one); any other letter is
 * pushed. The letters left on the stack can't be removed, in any order. */
int minLength(char *s)
{
    size_t len = strlen(s);
    char *stack = malloc(len + 1);
    int top = 0, i;

    for (i = 0; s[i]; i++) {
        if (top > 0 && ((s[i] == 'B' && stack[top - 1] == 'A') ||
                        (s[i] == 'D' && stack[top - 1] == 'C')))
            top--;
        else
            stack[top++] = s[i];
    }
    free(stack);
    return top;
}
