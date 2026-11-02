/* Slow and obviously right: replay each log into a buffer of its own,
 * key by key, and compare the two texts. */
static char *replay(const char *log)
{
    char *box = malloc(strlen(log) + 1);
    size_t n = 0;

    for (; *log; log++) {
        if (*log != '#')
            box[n++] = *log;
        else if (n > 0)
            n--;
    }
    box[n] = '\0';
    return box;
}

bool backspaceCompare(char *s, char *t)
{
    char *a = replay(s), *b = replay(t);
    bool same = strcmp(a, b) == 0;

    free(a);
    free(b);
    return same;
}
