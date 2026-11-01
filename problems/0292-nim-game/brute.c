/* Slow and obviously right: play the game out for every pile size from 0
 * up to n. A pile is won for the player to move when some move (1, 2 or 3
 * pebbles, no more than the pile holds) leaves a pile that is lost for the
 * opponent; the empty pile is lost. Only the last three results are
 * needed, so they are kept in a ring of four. */
bool canWinNim(int n)
{
    bool win[4];
    long long m;
    int t;

    win[0] = false;
    for (m = 1; m <= n; m++) {
        bool w = false;

        for (t = 1; t <= 3 && t <= m; t++)
            if (!win[(m - t) % 4])
                w = true;
        win[m % 4] = w;
    }
    return win[n % 4];
}
