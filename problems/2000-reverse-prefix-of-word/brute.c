/* Slow and obviously right: push letters onto a stack until ch has been
 * pushed, then pop the whole stack into the result and copy the rest. If
 * ch never comes, the stack is thrown away and word is copied as it is. */
char *reversePrefix(char *word, char ch)
{
    int n = (int)strlen(word), top = 0, len = 0, i = 0;
    char *stack = malloc((size_t)n + 1), *r = malloc((size_t)n + 1);

    while (i < n) {
        stack[top++] = word[i++];
        if (stack[top - 1] == ch)
            break;
    }
    if (top > 0 && stack[top - 1] == ch) {
        while (top > 0)
            r[len++] = stack[--top];
    } else {
        i = 0;
    }
    while (i < n)
        r[len++] = word[i++];
    r[len] = '\0';
    free(stack);
    return r;
}
