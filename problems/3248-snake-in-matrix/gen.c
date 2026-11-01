/* A random walk that never leaves the board: each move is drawn from the
 * directions open from the current square. Small cases use boards of 2 to
 * 4 and up to 8 moves; full-size ones use the 10 x 10 board and 100 moves.
 * Some walks lean toward one corner (so they run along the edges and end
 * in corners), the rest pick any open direction. */
void gen(int small)
{
    static char *names[4] = {"UP", "DOWN", "LEFT", "RIGHT"};
    char *commands[100];
    int n = small ? (int)rnd(2, 4) : (int)rnd(2, 10);
    int m = small ? (int)rnd(1, 8) : 100;
    int lean = (int)rnd(0, 4); /* 0..3: favour that direction; 4: none */
    int r = 0, c = 0, i, d, open[4], k;

    if (!small && rnd_chance(50))
        n = 10;
    for (i = 0; i < m; i++) {
        k = 0;
        if (r > 0)
            open[k++] = 0;
        if (r < n - 1)
            open[k++] = 1;
        if (c > 0)
            open[k++] = 2;
        if (c < n - 1)
            open[k++] = 3;
        d = open[rnd(0, k - 1)];
        if (lean < 4 && rnd_chance(60)) {
            int j;

            for (j = 0; j < k; j++)
                if (open[j] == lean)
                    d = lean;
        }
        if (d == 0)
            r--;
        else if (d == 1)
            r++;
        else if (d == 2)
            c--;
        else
            c++;
        commands[i] = names[d];
    }
    put_int(n);
    put_strings(commands, m);
}
