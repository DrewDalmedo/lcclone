/* Slow and obviously right: try every set of stamp positions (a stamp
 * used twice does nothing more, and the order doesn't matter), and keep
 * the smallest set that leaves no 'X'. Only for short strings. */
int minimumMoves(char *s)
{
    int n = (int)strlen(s), starts = n - 2, best = n, set, k, i, used;
    char covered[32];

    for (set = 0; set < 1 << starts; set++) {
        memset(covered, 0, sizeof covered);
        used = 0;
        for (k = 0; k < starts; k++)
            if (set >> k & 1) {
                used++;
                covered[k] = covered[k + 1] = covered[k + 2] = 1;
            }
        for (i = 0; i < n; i++)
            if (s[i] == 'X' && !covered[i])
                break;
        if (i == n && used < best)
            best = used;
    }
    return best;
}
