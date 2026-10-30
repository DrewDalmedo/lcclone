/* Reference solution: a stack of the brackets still open. */
bool isValid(char *s)
{
    size_t n = strlen(s), top = 0, i;
    char *stack = malloc(n + 1);
    bool ok = true;

    for (i = 0; i < n && ok; i++) {
        char ch = s[i];

        if (ch == '(' || ch == '[' || ch == '{') {
            stack[top++] = ch;
        } else {
            char open = ch == ')' ? '(' : ch == ']' ? '[' : '{';
            ok = top > 0 && stack[--top] == open;
        }
    }
    free(stack);
    return ok && top == 0;
}
