/* Slow and obviously right, by text replacement: replace every "()" with
 * "o", then every "(al)" with "al" (neither pattern occurs inside the
 * other, so the order of the two passes doesn't matter). */
static char *replace_all(const char *s, const char *from, const char *to)
{
    size_t flen = strlen(from), tlen = strlen(to), len = 0;
    char *out = malloc(strlen(s) + 1);

    while (*s) {
        if (strncmp(s, from, flen) == 0) {
            memcpy(out + len, to, tlen);
            len += tlen;
            s += flen;
        } else {
            out[len++] = *s++;
        }
    }
    out[len] = '\0';
    return out;
}

char *interpret(char *command)
{
    char *once = replace_all(command, "()", "o");
    char *twice = replace_all(once, "(al)", "al");

    free(once);
    return twice;
}
