/* A floor of 1 to 5 by 1 to 5 tiles for small cases; full-size ones are 50
 * by 50 most of the time, else of a random size up to that. The codes come
 * from a palette of 1 to 4 different values, 0 and 65535 among the likely
 * ones. The seed picks the layout:
 *   - noise (more than half the time): each tile has the first palette code
 *     with a chance picked by the seed, else another one, which makes
 *     patches of every shape;
 *   - one code everywhere;
 *   - a snake of one code winding through the rest, row by row or column
 *     by column, or a spiral with a one-tile gap between its turns.
 * The floor may then be mirrored across either axis. The start is any tile,
 * or an end of the snake or spiral; the new colour is often the starting
 * code itself, or another code of the floor. */
static int R, C;
static int *g;
static int pal[4], npal;
static int end_r[2], end_c[2];

static const int dr[] = {0, 1, 0, -1}, dc[] = {1, 0, -1, 0};

static int inside(int r, int c)
{
    return r >= 0 && r < R && c >= 0 && c < C;
}

static int pick_code(int small)
{
    int t = (int)rnd(0, 9);

    if (t == 0)
        return 0;
    if (t == 1)
        return 65535;
    return (int)(small ? rnd(0, 9) : rnd(0, 65535));
}

/* a palette code other than pal[0], or pal[0] + 1 when there is none */
static int other_code(void)
{
    int k = npal > 1 ? (int)rnd(1, npal - 1) : 0;

    if (pal[k] != pal[0])
        return pal[k];
    return (pal[0] + 1) % 65536;
}

/* the path along rows 0, 2, 4, ..., joined at alternate ends; with
 * by_cols, the same along columns */
static void snake(int by_cols)
{
    int i, j, a = by_cols ? C : R, b = by_cols ? R : C, path, last;

    for (i = 0; i < a; i++)
        for (j = 0; j < b; j++) {
            path = i % 2 == 0 || j == ((i / 2) % 2 == 0 ? b - 1 : 0);
            if (!path)
                continue;
            if (by_cols)
                g[j * C + i] = pal[0];
            else
                g[i * C + j] = pal[0];
        }
    /* the far end is in the last row (or column): at the end a full row
     * runs to, or at the joint that sticks out of the row before it */
    i = a - 1;
    last = (i / 2) % 2 == 0 ? b - 1 : 0;
    end_r[0] = 0;
    end_c[0] = 0;
    end_r[1] = by_cols ? last : i;
    end_c[1] = by_cols ? i : last;
}

/* from the top left corner, go straight on while the tile ahead is off the
 * path and the one after it isn't on it, else turn right; stop when
 * turning twice in a row doesn't help */
static void spiral(char *on)
{
    int r = 0, c = 0, d = 0, turns = 0, r1, c1, r2, c2;

    on[0] = 1;
    while (turns < 2) {
        r1 = r + dr[d];
        c1 = c + dc[d];
        r2 = r1 + dr[d];
        c2 = c1 + dc[d];
        if (inside(r1, c1) && !on[r1 * C + c1]
            && !(inside(r2, c2) && on[r2 * C + c2])) {
            r = r1;
            c = c1;
            on[r * C + c] = 1;
            turns = 0;
        } else {
            d = (d + 1) % 4;
            turns++;
        }
    }
    end_r[0] = 0;
    end_c[0] = 0;
    end_r[1] = r;
    end_c[1] = c;
}

void gen(int small)
{
    int shape = (int)rnd(0, 19), path = 0, i, j, k, v, sr, sc, color, old, pct;
    int flip_r, flip_c;
    int *out;
    char *on;

    if (small) {
        R = (int)rnd(1, 5);
        C = (int)rnd(1, 5);
    } else if (rnd_chance(60)) {
        R = 50;
        C = 50;
    } else {
        R = (int)rnd(1, 50);
        C = (int)rnd(1, 50);
    }
    npal = (int)rnd(1, 4);
    for (k = 0; k < npal; k++) {
        do {
            v = pick_code(small);
            for (j = 0; j < k && pal[j] != v; j++)
                ;
        } while (j < k);
        pal[k] = v;
    }
    g = malloc((size_t)R * C * sizeof *g);
    on = calloc((size_t)R * C, 1);

    if (shape <= 10) {
        pct = small ? (int)rnd(25, 70) : (int)rnd(30, 80);
        for (i = 0; i < R * C; i++) {
            if (rnd_chance(pct)) {
                g[i] = pal[0];
            } else {
                v = other_code();
                g[i] = v;
            }
        }
    } else if (shape == 11) {
        for (i = 0; i < R * C; i++)
            g[i] = pal[0];
    } else {
        for (i = 0; i < R * C; i++) {
            v = other_code();
            g[i] = v;
        }
        path = 1;
        if (shape >= 17) {
            spiral(on);
            for (i = 0; i < R * C; i++)
                if (on[i])
                    g[i] = pal[0];
        } else {
            snake(shape >= 15);
        }
    }

    if (path && rnd_chance(60)) {
        k = (int)rnd(0, 1);
        sr = end_r[k];
        sc = end_c[k];
    } else {
        sr = (int)rnd(0, R - 1);
        sc = (int)rnd(0, C - 1);
    }
    old = g[sr * C + sc];
    k = (int)rnd(0, 9);
    if (k <= 1) {
        color = old;
    } else if (k <= 4) {
        /* the code of another tile, if one differs from the start */
        j = (int)rnd(0, (long long)R * C - 1);
        color = g[j];
        if (color == old)
            color = old == pal[0] ? other_code() : pal[0];
    } else if (k == 5) {
        color = rnd_chance(50) ? 0 : 65535;
    } else {
        color = pick_code(small);
    }

    flip_r = rnd_chance(50);
    flip_c = rnd_chance(50);
    out = malloc((size_t)R * C * sizeof *out);
    for (i = 0; i < R; i++)
        for (j = 0; j < C; j++)
            out[i * C + j] = g[(flip_r ? R - 1 - i : i) * C
                               + (flip_c ? C - 1 - j : j)];
    if (flip_r)
        sr = R - 1 - sr;
    if (flip_c)
        sc = C - 1 - sc;
    put_int_grid(out, R, C);
    put_int(sr);
    put_int(sc);
    put_int(color);
    free(out);
    free(on);
    free(g);
}
