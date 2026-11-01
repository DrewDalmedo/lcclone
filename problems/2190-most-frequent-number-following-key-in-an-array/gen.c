/* A random log, then made to have one value written down most often (see
 * fix). Small cases have 2 to 8 entries of a few small values (sometimes
 * 1000), key among them at a rate the seed picks, so that runs of key and
 * near-ties are common. Full-size cases
 * have 1000 entries, in a shape the seed picks: many different values
 * after key (key never twice in a row), runs of key against a few other
 * values, key only a few times near the end, or two values written down
 * hundreds of times each, the winner more than 256 times. */
static int count[1001];

/* Counts how often each value comes right after key. Returns the top
 * count, and sets *ties to the number of values that have it. */
static int score(const int *a, int n, int key, int *ties)
{
    int i, v, top = 0;

    memset(count, 0, sizeof count);
    for (i = 0; i + 1 < n; i++)
        if (a[i] == key)
            count[a[i + 1]]++;
    *ties = 0;
    for (v = 1; v <= 1000; v++)
        if (count[v] > top) {
            top = count[v];
            *ties = 1;
        } else if (count[v] == top) {
            (*ties)++;
        }
    return top;
}

/* Breaks a tie for the top count by turning one entry a[i] != key into
 * key, where w = a[i + 1] is one of the tied values. That adds 1 to w's
 * count. It also moves 1 from a[i]'s count to key's when a[i - 1] is key,
 * so such an i is used only when w is key or key's count is below the
 * top; then key can't tie w. Returns 0 when no entry qualifies. */
static int ok(const int *a, int key, int top, int i)
{
    int w = a[i + 1];

    return a[i] != key && count[w] == top &&
           (w == key || count[key] < top || i == 0 || a[i - 1] != key);
}

static int fix(int *a, int n, int key)
{
    int ties, top = score(a, n, key, &ties), found = 0, pick, i;

    if (top > 0 && ties == 1)
        return 1;
    for (i = 0; i + 1 < n; i++)
        found += ok(a, key, top, i);
    if (found == 0)
        return 0;
    pick = (int)rnd(0, found - 1);
    for (i = 0; i + 1 < n; i++)
        if (ok(a, key, top, i) && pick-- == 0) {
            a[i] = key;
            break;
        }
    top = score(a, n, key, &ties);
    return top > 0 && ties == 1;
}

/* A value from 1 to 1000 other than key. */
static int other(int key)
{
    int v = (int)rnd(1, 999);

    return v >= key ? v + 1 : v;
}

/* key (here k, from 1 to top) at about pct% of the entries, the other
 * values from 1 to top; then top becomes 1000 when big. */
static void fill_small(int *a, int n, int k, int top, int big)
{
    int pct = (int)rnd(10, 40), i, v;

    for (i = 0; i < n; i++) {
        if (rnd_chance(pct)) {
            v = k;
        } else {
            v = (int)rnd(1, top - 1);
            if (v >= k)
                v++;
        }
        a[i] = big && v == top ? 1000 : v;
    }
}

static void fill_large(int *a, int n, int key, int shape)
{
    int pool[5], np, run, maxrun, cw, cr, tokens[500], nt, i, j, c;

    switch (shape) {
    case 0: /* key at about 30% of the entries, never twice in a row */
        for (i = 0; i < n; i++)
            a[i] = (i == 0 || a[i - 1] != key) && rnd_chance(30)
                       ? key
                       : other(key);
        break;
    case 1: /* runs of key, each followed by one of a few other values */
        np = (int)rnd(1, 5);
        for (j = 0; j < np; j++)
            pool[j] = other(key);
        maxrun = rnd_chance(50) ? (int)rnd(2, 5) : (int)rnd(6, 40);
        for (i = 0; i < n;) {
            run = (int)rnd(1, maxrun);
            for (j = 0; j < run && i < n; j++)
                a[i++] = key;
            if (i < n)
                a[i++] = pool[rnd(0, np - 1)];
        }
        break;
    case 2: /* key only 1 to 4 times, among the last 20 entries */
        for (i = 0; i < n; i++)
            a[i] = other(key);
        c = (int)rnd(1, 4);
        for (j = 0; j < c; j++)
            a[rnd(n - 20, n - 1)] = key;
        break;
    default: /* cw pairs (key, w), cr pairs (key, r), w more than 256 */
        cw = (int)rnd(257, 370);
        cr = (int)rnd(cw - 255, cw - 1 < 500 - cw ? cw - 1 : 500 - cw);
        nt = 0;
        for (j = 0; j < cw; j++)
            tokens[nt++] = 1;
        for (j = 0; j < cr; j++)
            tokens[nt++] = 2;
        rnd_shuffle(tokens, nt);
        pool[0] = other(key);
        do
            pool[1] = other(key);
        while (pool[1] == pool[0]);
        /* each pair goes at a random place among the other entries */
        c = n - 2 * nt;
        for (i = 0, j = 0; i < n;) {
            if (j < nt && (c == 0 || rnd_chance(50))) {
                a[i++] = key;
                a[i++] = pool[tokens[j++] - 1];
            } else {
                a[i++] = other(key);
                c--;
            }
        }
        break;
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 1000;
    int *a = malloc((size_t)n * sizeof *a);
    int top = small ? (int)rnd(2, 5) : 0;
    int big = small ? rnd_chance(20) : 0;
    int k = small ? (int)rnd(1, top) : (int)rnd(1, 1000);
    int key = big && k == top ? 1000 : k;
    int shape = small ? 0 : (int)rnd(0, 3);

    do {
        if (small)
            fill_small(a, n, k, top, big);
        else
            fill_large(a, n, key, shape);
    } while (!fix(a, n, key));
    put_ints(a, n);
    put_int(key);
    free(a);
}
