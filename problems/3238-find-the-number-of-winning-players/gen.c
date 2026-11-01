/* n players and a list of draws [x, y], in random order. The seed picks
 * the shape:
 *   0: random draws;
 *   1: each player gets exactly the balls of one colour needed to win, or
 *      one fewer, or (now and then) nothing, then more draws are added
 *      that never let a (player, colour) count reach a new verdict: a
 *      loser stays below the target in every colour, and a winner may
 *      reach it in other colours too;
 *   2: the same with every player losing, so the high players hold many
 *      balls spread over several colours;
 *   3: the same with every player winning.
 * Small cases have 2 to 5 players and up to 18 draws, with colours from a
 * pool of three (0, 10 and one more), so equal draws come up often;
 * full-size cases have 10 players and 100 draws over all 11 colours. */
static int cnt[10][11], cap[10][11];
static int xs[100], ys[100], len, pool[3], tiny;

static int colour(void)
{
    int j;

    if (tiny) {
        j = (int)rnd(0, 2);
        return pool[j];
    }
    return (int)rnd(0, 10);
}

static void add(int x, int y)
{
    xs[len] = x;
    ys[len] = y;
    len++;
    cnt[x][y]++;
}

void gen(int small)
{
    int shape = (int)rnd(0, 3);
    int n = small ? (int)rnd(2, 5) : 10;
    int target[10], tcol[10], order[100], out[200];
    int i, c, t, x, y, want, tries, win, quiet, top, total = 0;

    tiny = small;
    len = 0;
    memset(cnt, 0, sizeof cnt);
    pool[0] = 0;
    pool[1] = 10;
    pool[2] = (int)rnd(1, 9);
    if (shape == 0) {
        want = small ? (int)rnd(1, 10) : 100;
        for (i = 0; i < want; i++) {
            x = (int)rnd(0, n - 1);
            y = colour();
            add(x, y);
        }
    } else {
        for (i = 0; i < n; i++) {
            win = shape == 3 ? 1 : shape == 2 ? 0 : rnd_chance(50);
            quiet = !win && rnd_chance(15);
            if (i == 0 && !win)
                quiet = 1; /* player 0 loses only by drawing nothing */
            target[i] = quiet ? 0 : win ? i + 1 : i;
            tcol[i] = colour();
            top = quiet ? 0 : win ? i + 1 : i;
            for (c = 0; c <= 10; c++)
                cap[i][c] = top;
            total += target[i];
        }
        if (total == 0) {
            /* Nobody drew anything: let player 0 draw one ball. */
            target[0] = 1;
            for (c = 0; c <= 10; c++)
                cap[0][c] = 1;
        }
        for (i = 0; i < n; i++)
            for (t = 0; t < target[i]; t++)
                add(i, tcol[i]);
        if (small) {
            t = (int)rnd(0, 3);
            want = len + t;
        } else {
            want = 100;
        }
        for (tries = 0; len < want && tries < 10000; tries++) {
            x = (int)rnd(0, n - 1);
            y = colour();
            if (cnt[x][y] < cap[x][y])
                add(x, y);
        }
    }
    for (i = 0; i < len; i++)
        order[i] = i;
    rnd_shuffle(order, len);
    for (i = 0; i < len; i++) {
        out[2 * i] = xs[order[i]];
        out[2 * i + 1] = ys[order[i]];
    }
    put_int(n);
    put_int_grid(out, len, 2);
}
