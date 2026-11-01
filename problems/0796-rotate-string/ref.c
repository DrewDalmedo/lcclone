/* Reference solution: the rotations of s are exactly the strings of its
 * length that occur in s written twice in a row. So goal is one when the
 * lengths agree and goal occurs in s + s. */
bool rotateString(char *s, char *goal)
{
    size_t n = strlen(s);
    char *twice;
    bool found;

    if (strlen(goal) != n)
        return false;
    twice = malloc(2 * n + 1);
    memcpy(twice, s, n);
    memcpy(twice + n, s, n + 1);
    found = strstr(twice, goal) != NULL;
    free(twice);
    return found;
}
