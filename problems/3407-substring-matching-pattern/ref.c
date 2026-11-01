/* Reference solution: split p at the '*' into a left and a right part.
 * Find the leftmost copy of the left part in s (an empty part is found at
 * 0); it leaves the most room after it. Then look for the right part
 * anywhere from the end of that copy on. */
static int find_from(const char *s, int n, int from, const char *w, int k)
{
    int i;

    for (i = from; i + k <= n; i++)
        if (strncmp(s + i, w, (size_t)k) == 0)
            return i;
    return -1;
}

bool hasMatch(char *s, char *p)
{
    int n = (int)strlen(s);
    int star = (int)(strchr(p, '*') - p);
    int rlen = (int)strlen(p) - star - 1;
    int at = find_from(s, n, 0, p, star);

    if (at < 0)
        return false;
    return find_from(s, n, at + star, p + star + 1, rlen) >= 0;
}
