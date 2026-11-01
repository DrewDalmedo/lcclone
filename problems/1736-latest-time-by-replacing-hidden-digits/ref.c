/* Reference solution: greedy, digit by digit from the left, in a fresh
 * buffer. A hidden first hour digit is 2 unless the second one is a known
 * 4..9 (then 1); a hidden second hour digit is 3 after a 2, else 9; hidden
 * minute digits are 5 and 9. */
char *maximumTime(char *time)
{
    char *r = malloc(6);

    memcpy(r, time, 6);
    if (r[0] == '?')
        r[0] = (r[1] == '?' || r[1] <= '3') ? '2' : '1';
    if (r[1] == '?')
        r[1] = r[0] == '2' ? '3' : '9';
    if (r[3] == '?')
        r[3] = '5';
    if (r[4] == '?')
        r[4] = '9';
    return r;
}
