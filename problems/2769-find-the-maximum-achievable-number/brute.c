/* Slow and obviously right: try starting points from well above num
 * downwards. For each, play out every choice of moves round by round,
 * keeping the set of gaps x - num the two markers can have (each round
 * the x marker steps by -1 or +1 and the num marker by -1 or +1), and
 * return the first start from which a gap of 0 can be reached within t
 * rounds. */
int theMaximumAchievableX(int num, int t)
{
    enum { OFF = 512, W = 1025 };
    static char cur[W], nxt[W];
    int x, r, g, dx, dn;

    for (x = num + 4 * t + 2;; x--) {
        memset(cur, 0, sizeof cur);
        cur[x - num + OFF] = 1;
        for (r = 1; r <= t; r++) {
            memset(nxt, 0, sizeof nxt);
            for (g = 0; g < W; g++)
                if (cur[g])
                    for (dx = -1; dx <= 1; dx += 2)
                        for (dn = -1; dn <= 1; dn += 2)
                            if (g + dx - dn >= 0 && g + dx - dn < W)
                                nxt[g + dx - dn] = 1;
            if (nxt[OFF])
                return x;
            memcpy(cur, nxt, sizeof cur);
        }
    }
}
