/* Reference solution: fill the blanks greedily from the left, giving each
 * the largest digit that still leaves a valid time. The first hour digit
 * can be 1 unless the second one is fixed at 2 or more; after a 1 the
 * second hour digit is at most 1, after a 0 at most 9. Minutes are at most
 * 59, independently of the hour. */
char *findLatestTime(char *s)
{
    char *t = malloc(6);

    memcpy(t, s, 6);
    if (t[0] == '?')
        t[0] = (t[1] == '?' || t[1] <= '1') ? '1' : '0';
    if (t[1] == '?')
        t[1] = t[0] == '1' ? '1' : '9';
    if (t[3] == '?')
        t[3] = '5';
    if (t[4] == '?')
        t[4] = '9';
    return t;
}
