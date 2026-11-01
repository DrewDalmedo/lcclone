/* A random board of B and W. Only 14 of the 512 boards give false, so a
 * third of the time the board is drawn again until each of its four blocks
 * has two tiles of each color (about 37 draws on average). Otherwise one
 * color is often favoured, which brings up blocks that are already one
 * color. The board always has 3 x 3 tiles, so full-size cases are made the
 * same way. */
static int all_split(const char *g)
{
    int r, c, black;

    for (r = 0; r < 2; r++)
        for (c = 0; c < 2; c++) {
            black = (g[3 * r + c] == 'B') + (g[3 * r + c + 1] == 'B') +
                    (g[3 * r + c + 3] == 'B') + (g[3 * r + c + 4] == 'B');
            if (black != 2)
                return 0;
        }
    return 1;
}

void gen(int small)
{
    char g[9];
    int want_false = rnd_chance(33), black = 50, i;

    (void)small;
    if (!want_false && rnd_chance(50))
        black = (int)rnd(15, 85);
    do {
        for (i = 0; i < 9; i++)
            g[i] = rnd_chance(black) ? 'B' : 'W';
    } while (want_false && !all_split(g));
    put_char_grid(g, 3, 3);
}
