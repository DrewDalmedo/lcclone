/* Reference solution: one pass. zeros counts the 0s of the current group
 * of 0s and ones the 1s that have followed it; a 0 after some 1s starts a
 * new group. At every 1, the last min(zeros, ones) 0s and the 1s up to it
 * form a balanced substring. */
int findTheLongestBalancedSubstring(char *s)
{
    int zeros = 0, ones = 0, best = 0, m, i;

    for (i = 0; s[i]; i++) {
        if (s[i] == '0') {
            if (ones > 0) {
                zeros = 0;
                ones = 0;
            }
            zeros++;
        } else {
            ones++;
            m = ones < zeros ? ones : zeros;
            if (2 * m > best)
                best = 2 * m;
        }
    }
    return best;
}
