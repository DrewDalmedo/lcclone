/* Small cases: 1 to 12 letters over two or three letters, built run by
 * run, with k often the length of one of the runs, or one more than it.
 * Full-size ones have 100 letters, in a shape picked by the seed: random
 * runs of a and b; runs of every length but k (some of them k + 1 or 2k
 * long), so the answer is false; the same with a run of exactly k at the
 * very end; or a single letter with k = 100 or 99. */
static char other(const char *letters, char prev)
{
    char c;

    do
        c = rnd_char(letters);
    while (c == prev);
    return c;
}

/* Fills s[from..to) with runs of letters whose lengths are never k, each
 * letter different from the one before (prev, at first); returns the last
 * letter. A leftover piece of exactly k joins the run before it. */
static char runs_but_k(char *s, int from, int to, int k, char prev)
{
    static const char *az = "abcdefghijklmnopqrstuvwxyz";
    int len, i, pick;
    char c = prev;

    while (from < to) {
        pick = (int)rnd(0, 3);
        if (pick == 0)
            len = k + 1;
        else if (pick == 1)
            len = 2 * k;
        else
            len = (int)rnd(1, 2 * k + 1);
        if (len == k)
            len = k + 1;
        if (len > to - from)
            len = to - from;
        if (len != k || c == 0)
            c = other(az, c);
        for (i = 0; i < len; i++)
            s[from++] = c;
    }
    return c;
}

void gen(int small)
{
    char s[101], c = 0, last;
    int n, k, shape, len, i, j, pos;
    const char *letters;

    if (small) {
        letters = rnd_chance(50) ? "ab" : "abc";
        n = (int)rnd(1, 12);
        for (i = 0; i < n; i += len) {
            len = (int)rnd(1, 4);
            if (len > n - i)
                len = n - i;
            c = rnd_char(letters);
            for (j = 0; j < len; j++)
                s[i + j] = c;
        }
        s[n] = '\0';
        pos = (int)rnd(0, n - 1);
        for (i = pos; i > 0 && s[i - 1] == s[pos]; i--)
            ;
        for (j = pos; j < n && s[j] == s[pos]; j++)
            ;
        k = j - i;
        if (rnd_chance(30))
            k++;
        if (rnd_chance(20))
            k = (int)rnd(1, n);
        if (k > n)
            k = n;
        put_string(s);
        put_int(k);
        return;
    }
    n = 100;
    shape = (int)rnd(0, 3);
    k = (int)rnd(1, 20);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("ab");
        k = (int)rnd(1, 8);
    } else if (shape == 1) {
        runs_but_k(s, 0, n, k, 0);
    } else if (shape == 2) {
        last = runs_but_k(s, 0, n - k, k, 0);
        c = other("abcdefghijklmnopqrstuvwxyz", last);
        for (i = n - k; i < n; i++)
            s[i] = c;
    } else {
        c = rnd_char("abcdefghijklmnopqrstuvwxyz");
        for (i = 0; i < n; i++)
            s[i] = c;
        k = rnd_chance(50) ? 100 : 99;
    }
    s[n] = '\0';
    put_string(s);
    put_int(k);
}
