/* Small cases: up to 10 letters, either at random or as a walk that
 * avoids the corners it has seen for as long as it can, sometimes with
 * the last step forced back onto one of them. Full-size cases: 10^4
 * letters, as a random walk, or in shapes that never repeat a corner (a
 * staircase heading two ways, an outward spiral, a straight run, maybe
 * after one sideways step), so that nothing ends the search early;
 * some of them repeat a corner only with their very last step, and a
 * rectangle with a perimeter of 10^4 blocks meets its start only at the
 * end. */
static char opposite(char c)
{
    return c == 'N' ? 'S' : c == 'S' ? 'N' : c == 'E' ? 'W' : 'E';
}

static void step(char c, int *x, int *y)
{
    *x += (c == 'E') - (c == 'W');
    *y += (c == 'N') - (c == 'S');
}

static void gen_small(char *path)
{
    static const char dirs[] = "NSEW";
    char seen[21][21], fresh[4], old[4];
    int n = (int)rnd(1, 10), mode = (int)rnd(0, 2), x = 0, y = 0, i, d;
    int nf, no, nx, ny;

    memset(seen, 0, sizeof seen);
    seen[10][10] = 1;
    for (i = 0; i < n; i++) {
        nf = no = 0;
        for (d = 0; d < 4; d++) {
            nx = x;
            ny = y;
            step(dirs[d], &nx, &ny);
            if (seen[nx + 10][ny + 10])
                old[no++] = dirs[d];
            else
                fresh[nf++] = dirs[d];
        }
        if (mode == 0)
            path[i] = dirs[rnd(0, 3)];
        else if (mode == 2 && i == n - 1 && no > 0)
            path[i] = old[rnd(0, no - 1)];
        else if (nf > 0)
            path[i] = fresh[rnd(0, nf - 1)];
        else
            path[i] = old[rnd(0, no - 1)];
        step(path[i], &x, &y);
        seen[x + 10][y + 10] = 1;
    }
    path[n] = '\0';
}

static void gen_large(char *path)
{
    int n = 10000, shape = (int)rnd(0, 6), i, k, len, leg, turn;
    char v, h, c, side;
    const char *ring;

    if (shape == 0) {
        for (i = 0; i < n; i++)
            path[i] = rnd_char("NSEW");
    } else if (shape <= 2) {
        v = rnd_char("NS");
        h = rnd_char("EW");
        for (i = 0; i < n; i++)
            path[i] = rnd_chance(50) ? v : h;
        if (shape == 2)
            path[n - 1] = opposite(path[n - 2]);
    } else if (shape <= 4) {
        ring = rnd_chance(50) ? "ENWS" : "ESWN";
        turn = (int)rnd(0, 3);
        i = 0;
        for (len = 1; i < n; len++)
            for (leg = 0; leg < 2; leg++) {
                for (k = 0; k < len && i < n; k++)
                    path[i++] = ring[turn];
                turn = (turn + 1) % 4;
            }
        if (shape == 4)
            path[n - 1] = opposite(path[n - 2]);
    } else if (shape == 5) {
        c = rnd_char("NSEW");
        side = (c == 'N' || c == 'S') ? rnd_char("EW") : rnd_char("NS");
        for (i = 0; i < n; i++)
            path[i] = c;
        if (rnd_chance(50))
            path[0] = side;
    } else {
        /* leg blocks one way, len across, then both back again */
        v = rnd_char("NS");
        h = rnd_char("EW");
        leg = (int)rnd(1, n / 2 - 1);
        len = n / 2 - leg;
        i = 0;
        for (k = 0; k < leg; k++)
            path[i++] = v;
        for (k = 0; k < len; k++)
            path[i++] = h;
        for (k = 0; k < leg; k++)
            path[i++] = opposite(v);
        for (k = 0; k < len; k++)
            path[i++] = opposite(h);
    }
    path[n] = '\0';
}

void gen(int small)
{
    char *path = malloc(10001);

    if (small)
        gen_small(path);
    else
        gen_large(path);
    put_string(path);
    free(path);
}
