/* Lists built around a set of common numbers: every list gets the common
 * numbers plus extras of its own, and is then shuffled. Small cases have 1
 * to 5 lists drawn from a pool of up to 6 numbers (from 1..8, or from 1..4
 * and 997..1000), so that numbers in all lists but one, and empty answers,
 * come up often. Full-size ones hold 1000 numbers in all, with the seed
 * picking the shape: two lists of 500 that overlap a lot; one list of all
 * 1000 numbers; 1000 lists of one number each, all the same or all but
 * one; or 3 to 200 lists, one of them long and the rest short. */
static int flat[1000], len[1000], perm[1000];

static void put_lists(int n)
{
    int i, j, at = 0;

    putchar('[');
    for (i = 0; i < n; i++) {
        printf("%s[", i ? "," : "");
        for (j = 0; j < len[i]; j++)
            printf("%s%d", j ? "," : "", flat[at + j]);
        putchar(']');
        at += len[i];
    }
    puts("]");
}

static void gen_small(void)
{
    static const int edges[8] = {1, 2, 3, 4, 997, 998, 999, 1000};
    int pool[8], common[8], which[8];
    int p, n, nc = 0, drop_row = -1, drop = -1, at = 0, i, k;

    for (k = 0; k < 8; k++)
        pool[k] = k + 1;
    if (rnd_chance(30))
        for (k = 0; k < 8; k++)
            pool[k] = edges[k];
    rnd_shuffle(pool, 8);
    p = (int)rnd(1, 6);
    n = (int)rnd(1, 5);
    for (k = 0; k < p; k++) {
        common[k] = rnd_chance(40);
        if (common[k])
            which[nc++] = k;
    }
    if (n >= 2 && nc > 0 && rnd_chance(40)) {
        drop_row = (int)rnd(0, n - 1);
        k = (int)rnd(0, nc - 1);
        drop = which[k];
    }
    for (i = 0; i < n; i++) {
        len[i] = 0;
        for (k = 0; k < p; k++) {
            int in = common[k] || rnd_chance(45);

            if (i == drop_row && k == drop)
                in = 0;
            if (in)
                flat[at + len[i]++] = pool[k];
        }
        if (len[i] == 0) {
            k = (int)rnd(0, p - 1);
            flat[at + len[i]++] = pool[k];
        }
        rnd_shuffle(flat + at, len[i]);
        at += len[i];
    }
    put_lists(n);
}

/* Puts c common numbers and e extras (picked from perm[c..1000), which is
 * left a permutation) into list i, starting at flat[at], and shuffles it. */
static int fill(int i, int at, int c, int e)
{
    int t, j, x;

    len[i] = 0;
    for (t = 0; t < c; t++)
        flat[at + len[i]++] = perm[t];
    for (t = 0; t < e; t++) {
        j = (int)rnd(c + t, 999);
        x = perm[c + t];
        perm[c + t] = perm[j];
        perm[j] = x;
        flat[at + len[i]++] = perm[c + t];
    }
    rnd_shuffle(flat + at, len[i]);
    return at + len[i];
}

void gen(int small)
{
    int shape, n, c, i, at = 0, longest, rest, v, w;
    int extra[200];

    if (small) {
        gen_small();
        return;
    }
    for (i = 0; i < 1000; i++)
        perm[i] = i + 1;
    rnd_shuffle(perm, 1000);
    if (rnd_chance(50)) {
        /* make 1 and 1000 the first two common numbers */
        for (i = 0; i < 1000; i++)
            if (perm[i] == 1) {
                perm[i] = perm[0];
                perm[0] = 1;
            }
        for (i = 0; i < 1000; i++)
            if (perm[i] == 1000) {
                perm[i] = perm[1];
                perm[1] = 1000;
            }
    }
    shape = (int)rnd(0, 3);
    if (shape == 0) {
        n = 2;
        c = (int)rnd(250, 500);
        at = fill(0, at, c, 500 - c);
        fill(1, at, c, 500 - c);
    } else if (shape == 1) {
        n = 1;
        fill(0, 0, 0, 1000);
    } else if (shape == 2) {
        n = 1000;
        i = (int)rnd(0, 1);
        v = perm[i];
        w = perm[1 - i];
        for (i = 0; i < n; i++) {
            len[i] = 1;
            flat[i] = v;
        }
        if (rnd_chance(50)) {
            i = (int)rnd(0, n - 1);
            flat[i] = w;
        }
    } else {
        n = (int)rnd(3, 200);
        c = (int)rnd(0, 3);
        longest = (int)rnd(0, n - 1);
        rest = 1000 - c * n;
        for (i = 0; i < n; i++) {
            extra[i] = 0;
            if (i != longest) {
                extra[i] = c > 0 ? (int)rnd(0, 1) : 1;
                rest -= extra[i];
            }
        }
        extra[longest] = rest;
        for (i = 0; i < n; i++)
            at = fill(i, at, c, extra[i]);
    }
    put_lists(n);
}
