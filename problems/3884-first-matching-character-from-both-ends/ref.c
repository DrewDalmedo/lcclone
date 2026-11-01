/* Reference solution: two indices walk in from both ends and stop at the
 * first pair that holds the same letter. They meet at the middle square
 * of an odd-length string, which always matches. */
int firstMatchingIndex(char *s)
{
    int lo = 0, hi = (int)strlen(s) - 1;

    while (lo <= hi) {
        if (s[lo] == s[hi])
            return lo;
        lo++;
        hi--;
    }
    return -1;
}
