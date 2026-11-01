/* Reference solution: greedy, left to right. The leftmost 'X' still left
 * must be covered by some stamp, and the one that starts at it reaches
 * furthest right; so stamp there and go on three places later. (Near the
 * end the stamp actually used is the last one, which covers the same
 * 'X's.) */
int minimumMoves(char *s)
{
    int n = (int)strlen(s), stamps = 0, i = 0;

    while (i < n) {
        if (s[i] == 'X') {
            stamps++;
            i += 3;
        } else {
            i++;
        }
    }
    return stamps;
}
