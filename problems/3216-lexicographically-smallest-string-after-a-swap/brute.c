/* Slow and obviously right: start from s itself (no exchange), try every
 * allowed exchange of neighbours on a fresh copy, and keep the smallest
 * string seen. */
char *getSmallestString(char *s)
{
    size_t n = strlen(s), i;
    char *best = malloc(n + 1), *cand = malloc(n + 1), c;
    int even_a, even_b;

    memcpy(best, s, n + 1);
    for (i = 0; i + 1 < n; i++) {
        even_a = strchr("02468", s[i]) != NULL;
        even_b = strchr("02468", s[i + 1]) != NULL;
        if (even_a != even_b)
            continue;
        memcpy(cand, s, n + 1);
        c = cand[i];
        cand[i] = cand[i + 1];
        cand[i + 1] = c;
        if (strcmp(cand, best) < 0)
            memcpy(best, cand, n + 1);
    }
    free(cand);
    return best;
}
