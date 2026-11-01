/* Slow and obviously right: the fewest presses to move the clock ahead by
 * m minutes, for every m from 0 up, by dynamic programming over the last
 * button pressed. No greedy argument needed. */
int convertTime(char *current, char *correct)
{
    static const int step[4] = {1, 5, 15, 60};
    int best[1440];
    int h1, m1, h2, m2, d, m, i;

    sscanf(current, "%2d:%2d", &h1, &m1);
    sscanf(correct, "%2d:%2d", &h2, &m2);
    d = (h2 * 60 + m2) - (h1 * 60 + m1);
    best[0] = 0;
    for (m = 1; m <= d; m++) {
        best[m] = m;
        for (i = 0; i < 4; i++)
            if (step[i] <= m && best[m - step[i]] + 1 < best[m])
                best[m] = best[m - step[i]] + 1;
    }
    return best[d];
}
