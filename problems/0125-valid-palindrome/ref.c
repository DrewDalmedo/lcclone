/* Reference solution: two indexes move toward each other, each skipping
 * characters that are not letters or digits, and the characters they
 * stop at must match once lowercased. */
static int keep(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9');
}

static char lower(char c)
{
    return c >= 'A' && c <= 'Z' ? (char)(c - 'A' + 'a') : c;
}

bool isPalindrome(char *s)
{
    size_t lo = 0, hi = strlen(s);

    while (lo < hi) {
        if (!keep(s[lo])) {
            lo++;
        } else if (!keep(s[hi - 1])) {
            hi--;
        } else {
            if (lower(s[lo]) != lower(s[hi - 1]))
                return false;
            lo++;
            hi--;
        }
    }
    return true;
}
