/* Slow and obviously right: try every exchange of two positions i <= j
 * in a copy of s1 (i == j is the same as no exchange) and see whether the
 * copy reads like s2. */
bool areAlmostEqual(char *s1, char *s2)
{
    int n = (int)strlen(s1), i, j;
    char *t = malloc((size_t)n + 1), c;
    bool found = false;

    for (i = 0; i < n && !found; i++)
        for (j = i; j < n && !found; j++) {
            memcpy(t, s1, (size_t)n + 1);
            c = t[i];
            t[i] = t[j];
            t[j] = c;
            if (strcmp(t, s2) == 0)
                found = true;
        }
    free(t);
    return found;
}
