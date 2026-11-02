/* Slow and obviously right: build each of the n shifts and count the
 * doubles inside it. */
int countRotations(char *s, int k)
{
    int n = (int)strlen(s), p, i, doubles, count = 0;
    char *t = malloc((size_t)n + 1);

    for (p = 0; p < n; p++) {
        memcpy(t, s + p, (size_t)(n - p));
        memcpy(t + (n - p), s, (size_t)p);
        t[n] = '\0';
        doubles = 0;
        for (i = 0; i + 1 < n; i++)
            if (t[i] == t[i + 1])
                doubles++;
        if (doubles == k)
            count++;
    }
    free(t);
    return count;
}
