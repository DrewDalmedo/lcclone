/* Slow and obviously right: try every pair of letters that occur in s,
 * counting each anew, and keep the best odd count minus even count. */
static int occurrences(const char *s, char c)
{
    int n = 0;

    for (; *s; s++)
        if (*s == c)
            n++;
    return n;
}

int maxDifference(char *s)
{
    int best = 0, found = 0, x, y, fx, fy;

    for (x = 'a'; x <= 'z'; x++)
        for (y = 'a'; y <= 'z'; y++) {
            fx = occurrences(s, (char)x);
            fy = occurrences(s, (char)y);
            if (fx > 0 && fy > 0 && fx % 2 == 1 && fy % 2 == 0
                && (!found || fx - fy > best)) {
                best = fx - fy;
                found = 1;
            }
        }
    return best;
}
