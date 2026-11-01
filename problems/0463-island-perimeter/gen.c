/* A map of 1 to 6 by 1 to 6 cells for small cases; full-size ones are 100
 * by 100 most of the time, else of a random size up to that. The seed picks
 * the island's shape:
 *   - a blob grown from one cell, adding a random water neighbour of the
 *     land at a time, until it has a random number of cells (1 included);
 *   - a branching island grown the same way, but only through water cells
 *     that touch exactly one land cell, so that it has no 2 by 2 blocks and
 *     a long coast;
 *   - a comb: teeth one cell apart, joined along one edge of the map;
 *   - a spiral, with a one-cell channel of water between its turns;
 *   - the whole map, or a rectangle of it.
 * Every water cell that can't reach the edge of the map through water then
 * becomes land, so the island has no lakes, and the map may be mirrored
 * across either axis. */
static int R, C;
static int *g;

static const int dr[] = {0, 1, 0, -1}, dc[] = {1, 0, -1, 0};

static int inside(int r, int c)
{
    return r >= 0 && r < R && c >= 0 && c < C;
}

static int land_around(int r, int c)
{
    int k, cnt = 0;

    for (k = 0; k < 4; k++)
        if (inside(r + dr[k], c + dc[k]) && g[(r + dr[k]) * C + c + dc[k]])
            cnt++;
    return cnt;
}

static void grow(int target, int thin)
{
    int *cand = malloc(((size_t)4 * R * C + 1) * sizeof *cand);
    int n = 0, land = 0, i, k, cell, r, c, nr, nc;

    cand[n++] = (int)rnd(0, (long long)R * C - 1);
    while (land < target && n > 0) {
        i = (int)rnd(0, n - 1);
        cell = cand[i];
        cand[i] = cand[--n];
        r = cell / C;
        c = cell % C;
        if (g[cell] || (thin && land > 0 && land_around(r, c) != 1))
            continue;
        g[cell] = 1;
        land++;
        for (k = 0; k < 4; k++) {
            nr = r + dr[k];
            nc = c + dc[k];
            if (inside(nr, nc) && !g[nr * C + nc])
                cand[n++] = nr * C + nc;
        }
    }
    free(cand);
}

/* teeth along the columns 0, 2, 4, ..., standing on the bottom row, or
 * along the rows 0, 2, 4, ..., standing on the right column */
static void comb(int full)
{
    int vertical = rnd_chance(50), i, j, len;

    if (vertical) {
        for (j = 0; j < C; j++)
            g[(R - 1) * C + j] = 1;
        for (j = 0; j < C; j += 2) {
            len = full ? R - 1 : (int)rnd(0, R - 1);
            for (i = 1; i <= len; i++)
                g[(R - 1 - i) * C + j] = 1;
        }
    } else {
        for (i = 0; i < R; i++)
            g[i * C + C - 1] = 1;
        for (i = 0; i < R; i += 2) {
            len = full ? C - 1 : (int)rnd(0, C - 1);
            for (j = 1; j <= len; j++)
                g[i * C + C - 1 - j] = 1;
        }
    }
}

/* from the top left corner, go straight on while the cell ahead is water
 * and the one after it isn't land, else turn right; stop when turning
 * twice in a row doesn't help */
static void spiral(void)
{
    int r = 0, c = 0, d = 0, turns = 0, r1, c1, r2, c2;

    g[0] = 1;
    while (turns < 2) {
        r1 = r + dr[d];
        c1 = c + dc[d];
        r2 = r1 + dr[d];
        c2 = c1 + dc[d];
        if (inside(r1, c1) && !g[r1 * C + c1]
            && !(inside(r2, c2) && g[r2 * C + c2])) {
            r = r1;
            c = c1;
            g[r * C + c] = 1;
            turns = 0;
        } else {
            d = (d + 1) % 4;
            turns++;
        }
    }
}

static void fill_lakes(void)
{
    int *queue = malloc((size_t)R * C * sizeof *queue);
    char *wet = calloc((size_t)R * C, 1);
    int head = 0, tail = 0, i, j, k, r, c, nr, nc;

    for (i = 0; i < R; i++)
        for (j = 0; j < C; j++)
            if ((i == 0 || i == R - 1 || j == 0 || j == C - 1)
                && !g[i * C + j]) {
                wet[i * C + j] = 1;
                queue[tail++] = i * C + j;
            }
    while (head < tail) {
        r = queue[head] / C;
        c = queue[head] % C;
        head++;
        for (k = 0; k < 4; k++) {
            nr = r + dr[k];
            nc = c + dc[k];
            if (inside(nr, nc) && !g[nr * C + nc] && !wet[nr * C + nc]) {
                wet[nr * C + nc] = 1;
                queue[tail++] = nr * C + nc;
            }
        }
    }
    for (i = 0; i < R * C; i++)
        if (!wet[i])
            g[i] = 1;
    free(queue);
    free(wet);
}

void gen(int small)
{
    int shape = (int)rnd(0, 11), flip_r, flip_c, i, j, r0, r1, c0, c1, full;
    int *out;

    if (small) {
        R = (int)rnd(1, 6);
        C = (int)rnd(1, 6);
    } else if (rnd_chance(60)) {
        R = 100;
        C = 100;
    } else {
        R = (int)rnd(1, 100);
        C = (int)rnd(1, 100);
    }
    g = calloc((size_t)R * C, sizeof *g);
    if (shape <= 4) {
        grow((int)rnd(1, (long long)R * C), 0);
    } else if (shape <= 6) {
        grow(R * C, 1);
    } else if (shape == 7) {
        full = rnd_chance(small ? 50 : 70);
        comb(full);
    } else if (shape == 8) {
        spiral();
    } else if (shape == 9) {
        for (i = 0; i < R * C; i++)
            g[i] = 1;
    } else {
        r0 = (int)rnd(0, R - 1);
        r1 = (int)rnd(r0, R - 1);
        c0 = (int)rnd(0, C - 1);
        c1 = (int)rnd(c0, C - 1);
        for (i = r0; i <= r1; i++)
            for (j = c0; j <= c1; j++)
                g[i * C + j] = 1;
    }
    fill_lakes();
    flip_r = rnd_chance(50);
    flip_c = rnd_chance(50);
    out = malloc((size_t)R * C * sizeof *out);
    for (i = 0; i < R; i++)
        for (j = 0; j < C; j++)
            out[i * C + j] = g[(flip_r ? R - 1 - i : i) * C
                               + (flip_c ? C - 1 - j : j)];
    put_int_grid(out, R, C);
    free(out);
    free(g);
}
