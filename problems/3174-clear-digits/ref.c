/* Reference solution: use the output buffer as a stack. Push each letter;
 * a digit pops the letter on top (the closest one still there on its
 * left) and is dropped itself. */
char *clearDigits(char *s)
{
    size_t len = strlen(s), top = 0, i;
    char *out = malloc(len + 1);

    for (i = 0; i < len; i++) {
        if (s[i] >= '0' && s[i] <= '9')
            top--;
        else
            out[top++] = s[i];
    }
    out[top] = '\0';
    return out;
}
