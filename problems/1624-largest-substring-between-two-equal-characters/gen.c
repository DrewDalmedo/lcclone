/* A string over a shuffled alphabet. The seed picks a shape: random
 * letters from a few (small) or any number of letters; runs, where each
 * letter fills one stretch of the string, so the gaps are short; and runs
 * with one more letter sprinkled at a few places, whose first and last
 * copies make the answer while its neighbouring copies stand closer.
 * The third shape differs by size. Small cases (1 to 10 letters) have all
 * letters different, perhaps with one letter copied over another.
 * Full-size cases have 300 letters, and there it is short runs followed
 * by the widest gap, from a position past 255 to the last one, which a
 * byte-sized position table can't hold. */

/* Fills s[from, to) with one run of each of the k letters lt[0..k-1]:
 * runs of equal lengths, or of random ones (each at least 1). */
static void runs(char *s, int from, int to, const int *lt, int k, int equal)
{
    int cut[301], len = to - from, i, r;

    if (equal) {
        for (i = 0; i <= k; i++)
            cut[i] = from + i * len / k;
    } else {
        int pos[300];

        for (i = 0; i < len - 1; i++)
            pos[i] = i + 1;
        rnd_shuffle(pos, len - 1);
        cut[0] = 0;
        for (i = 1; i < k; i++)
            cut[i] = pos[i - 1];
        cut[k] = len;
        for (i = 1; i < k; i++)             /* sort the cuts */
            for (r = i; r > 1 && cut[r - 1] > cut[r]; r--) {
                int t = cut[r];
                cut[r] = cut[r - 1];
                cut[r - 1] = t;
            }
        for (i = 0; i <= k; i++)
            cut[i] += from;
    }
    for (i = 0; i < k; i++)
        for (r = cut[i]; r < cut[i + 1]; r++)
            s[r] = (char)('a' + lt[i]);
}

void gen(int small)
{
    char s[301];
    int lt[26], n = small ? (int)rnd(1, 10) : 300;
    int shape = (int)rnd(0, 3), k, m, i, j, src, p;

    for (i = 0; i < 26; i++)
        lt[i] = i;
    rnd_shuffle(lt, 26);
    if (shape == 0) {
        k = small ? (int)rnd(1, 4) : (int)rnd(1, 26);
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + lt[rnd(0, k - 1)]);
    } else if (shape == 1) {
        k = (int)rnd(1, n < 26 ? n : 26);
        runs(s, 0, n, lt, k, 0);
    } else if (shape == 2 && small) {
        for (i = 0; i < n; i++)
            s[i] = (char)('a' + lt[i]);
        if (rnd_chance(50)) {
            j = (int)rnd(0, n - 1);
            src = (int)rnd(0, n - 1);
            s[j] = s[src];
        }
    } else if (shape == 2) {
        p = (int)rnd(256, 275);
        runs(s, 0, p, lt + 1, 24, 1);
        s[p] = s[n - 1] = (char)('a' + lt[0]);
        for (i = p + 1; i < n - 1; i++)
            s[i] = (char)('a' + (rnd_chance(30) ? lt[0] : lt[25]));
    } else {
        k = (int)rnd(1, n < 25 ? n : 25);
        runs(s, 0, n, lt + 1, k, 0);
        m = small ? (int)rnd(2, 3) : (int)rnd(2, 8);
        for (i = 0; i < m; i++) {
            j = (int)rnd(0, n - 1);
            s[j] = (char)('a' + lt[0]);
        }
    }
    s[n] = '\0';
    put_string(s);
}
