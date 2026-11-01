/* Slow and obviously right: on a copy of s, find the first twin pair,
 * cut it out by moving the rest of the word two places left, and start
 * searching again from the beginning, until no twin pair is found. */
char *removeDuplicates(char *s)
{
    size_t n = strlen(s), i;
    char *w = malloc(n + 1);

    memcpy(w, s, n + 1);
    for (i = 0; i + 1 < n;) {
        if (w[i] == w[i + 1]) {
            memmove(w + i, w + i + 2, n - i - 1);
            n -= 2;
            i = 0;
        } else {
            i++;
        }
    }
    return w;
}
