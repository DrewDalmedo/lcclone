/* Slow and obviously right: try every pair of positions i < j, swap them in
 * a copy of s, and compare the copy with goal. */
bool buddyStrings(char *s, char *goal)
{
    size_t n = strlen(s), i, j;
    char *t, c;
    bool found = false;

    if (n != strlen(goal))
        return false;
    t = malloc(n + 1);
    for (i = 0; i < n && !found; i++)
        for (j = i + 1; j < n && !found; j++) {
            memcpy(t, s, n + 1);
            c = t[i];
            t[i] = t[j];
            t[j] = c;
            if (strcmp(t, goal) == 0)
                found = true;
        }
    free(t);
    return found;
}
