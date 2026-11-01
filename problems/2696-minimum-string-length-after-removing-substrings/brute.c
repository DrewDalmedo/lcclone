/* Slow and obviously right: on a copy of s, find a removable pair, delete
 * it, and start the search again, until no pair is left. */
int minLength(char *s)
{
    size_t len = strlen(s);
    char *t = malloc(len + 1);
    char *ab, *cd, *hit;
    int left;

    memcpy(t, s, len + 1);
    for (;;) {
        ab = strstr(t, "AB");
        cd = strstr(t, "CD");
        hit = ab ? ab : cd;
        if (!hit)
            break;
        memmove(hit, hit + 2, strlen(hit + 2) + 1);
    }
    left = (int)strlen(t);
    free(t);
    return left;
}
