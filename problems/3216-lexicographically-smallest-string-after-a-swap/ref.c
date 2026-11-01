/* Reference solution: greedy. An exchange helps only when the left digit is
 * bigger, and it leaves everything before position i alone while lowering
 * position i, so the leftmost neighbouring pair of the same parity with
 * s[i] > s[i + 1] is the one to swap. If there is none, s is the answer. */
char *getSmallestString(char *s)
{
    size_t n = strlen(s), i;
    char *t = malloc(n + 1), c;

    memcpy(t, s, n + 1);
    for (i = 0; i + 1 < n; i++)
        if ((t[i] - t[i + 1]) % 2 == 0 && t[i] > t[i + 1]) {
            c = t[i];
            t[i] = t[i + 1];
            t[i + 1] = c;
            break;
        }
    return t;
}
