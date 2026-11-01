/* Slow and obviously right: turn a copy of s step by step, moving its
 * first letter to its end, and compare it with goal before each step,
 * until every rotation has been seen. */
bool rotateString(char *s, char *goal)
{
    size_t n = strlen(s), k;
    char *copy = malloc(n + 1), first;
    bool found = false;

    strcpy(copy, s);
    for (k = 0; k < n && !found; k++) {
        if (strcmp(copy, goal) == 0)
            found = true;
        first = copy[0];
        memmove(copy, copy + 1, n - 1);
        copy[n - 1] = first;
    }
    free(copy);
    return found;
}
