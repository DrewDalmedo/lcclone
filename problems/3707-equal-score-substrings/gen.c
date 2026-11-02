/* Letters are handled as their worth v[i], 1 to 26. The seed picks the
 * shape: random letters; a planted balance point, made by moving letters
 * up or down one point at a time until the two sides of a chosen split
 * score the same; or a near miss, a balanced string with one letter moved
 * by one point, so that the total is odd and some prefix scores exactly
 * half of it, rounded down. Small cases have two to ten letters, mostly
 * from a to d. Full-size ones have 90 to 100, and may put the balance point
 * a few letters from either end. */

/* A random position in [from, to) whose worth can still go up (raise) or
 * down, or -1 if there is none. */
static int pick(const int *v, int from, int to, int raise)
{
    int cand[100], c = 0, i;

    for (i = from; i < to; i++)
        if (raise ? v[i] < 26 : v[i] > 1)
            cand[c++] = i;
    return c ? cand[rnd(0, c - 1)] : -1;
}

/* Moves letters one point at a time until the first cut letters and the
 * rest score the same. That is always possible when
 * cut <= 26 * (n - cut) and n - cut <= 26 * cut. */
static void balance(int *v, int n, int cut)
{
    int d = 0, i, p;

    for (i = 0; i < n; i++)
        d += i < cut ? v[i] : -v[i];
    while (d != 0) {
        if (d > 0) {
            p = pick(v, cut, n, 1);
            if (p >= 0)
                v[p]++;
            else
                v[pick(v, 0, cut, 0)]--;
            d--;
        } else {
            p = pick(v, 0, cut, 1);
            if (p >= 0)
                v[p]++;
            else
                v[pick(v, cut, n, 0)]--;
            d++;
        }
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 10) : (int)rnd(90, 100);
    int shape = small ? (int)rnd(0, 2) : (int)rnd(0, 3);
    int lo = (n + 26) / 27, hi = n - lo, cut = (int)rnd(lo, hi);
    int v[100], i, p;
    char s[101];

    if (shape == 2 && !small) {
        cut = rnd_chance(50) ? (int)rnd(lo, lo + 2) : (int)rnd(hi - 2, hi);
        for (i = 0; i < n; i++) {
            if ((i < cut) == (cut <= n / 2))
                v[i] = (int)rnd(1, 26);
            else
                v[i] = (int)rnd(1, 3);
        }
    } else {
        for (i = 0; i < n; i++)
            v[i] = small ? (int)rnd(1, 4) : (int)rnd(1, 26);
    }
    if (shape >= 1)
        balance(v, n, cut);
    if ((small && shape == 2) || (!small && shape == 3)) {
        p = pick(v, cut, n, 1);
        if (p >= 0)
            v[p]++;
        else
            v[pick(v, 0, cut, 0)]--;
    }
    for (i = 0; i < n; i++)
        s[i] = (char)('a' + v[i] - 1);
    s[n] = '\0';
    put_string(s);
}
