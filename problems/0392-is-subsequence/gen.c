/* Small cases: t has up to 10 letters from a tiny alphabet, and s is
 * picked from t's letters in order (sometimes with one letter changed or
 * two neighbours swapped afterwards), or is random. Full-size cases: t has
 * 10^4 letters and s 100, in a shape the seed picks: s is spread over
 * random t and ends at its last letter (true); s is 99 copies of t's only
 * letter and then another letter (false, after many partial matches); t
 * is built around s so that s fits only by using the whole of t, either
 * with s's last letter at t's end (true) or with a last letter that occurs
 * in t, but only in its first half (false); s and t are random over a and
 * b; or s is 100 copies of a letter that t holds only in its last 100
 * places (true). */

/* One position of t from each block of 100, the last one t's end. */
static void spread(char *s, const char *t)
{
    int k;

    for (k = 0; k < 99; k++)
        s[k] = t[k * 100 + rnd(0, 99)];
    s[99] = t[9999];
}

/* Before each of s's first 99 letters, t gets 100 letters other than it,
 * so each letter of s matches at the earliest at the end of its block.
 * perm[25] occurs only in the first 50 blocks and never in s[0..98]. */
static void tight(char *s, char *t, const int *perm, int fits)
{
    int k, i, pos = 0;
    char c;

    for (k = 0; k < 99; k++) {
        s[k] = (char)perm[rnd(0, 24)];
        for (i = 0; i < 100; i++) {
            do
                c = (char)perm[rnd(0, k < 50 ? 25 : 24)];
            while (c == s[k]);
            t[pos++] = c;
        }
        t[pos++] = s[k];
    }
    t[pos] = (char)perm[rnd(0, 24)];
    s[99] = fits ? t[pos] : (char)perm[25];
}

void gen(int small)
{
    static const char *alphabets[] = {
        "a", "ab", "abc", "az", "xyz", "abcdefghijklmnopqrstuvwxyz"
    };
    const char *abc;
    int m, n = 0, i, k, shape, perm[26];
    char *t, s[101], c;

    if (small) {
        abc = alphabets[rnd(0, 5)];
        m = (int)rnd(0, 10);
        t = malloc((size_t)m + 1);
        for (i = 0; i < m; i++)
            t[i] = rnd_char(abc);
        shape = (int)rnd(0, 5);
        if (shape <= 3) {
            for (i = 0; i < m; i++)
                if (rnd_chance(50))
                    s[n++] = t[i];
            if (shape == 2 && n > 0) {
                k = (int)rnd(0, n - 1);
                s[k] = rnd_char(abc);
            } else if (shape == 3 && n > 1) {
                k = (int)rnd(0, n - 2);
                c = s[k];
                s[k] = s[k + 1];
                s[k + 1] = c;
            }
        } else {
            n = (int)rnd(0, 6);
            for (i = 0; i < n; i++)
                s[i] = rnd_char(abc);
        }
    } else {
        m = 10000;
        n = 100;
        t = malloc((size_t)m + 1);
        shape = (int)(gen_seed() % 6);
        for (i = 0; i < 26; i++)
            perm[i] = 'a' + i;
        rnd_shuffle(perm, 26);
        if (shape == 0) {
            for (i = 0; i < m; i++)
                t[i] = (char)perm[rnd(0, 25)];
            spread(s, t);
        } else if (shape == 1) {
            memset(t, perm[0], (size_t)m);
            memset(s, perm[0], 99);
            s[99] = (char)perm[1];
        } else if (shape == 2 || shape == 3) {
            tight(s, t, perm, shape == 2);
        } else if (shape == 4) {
            for (i = 0; i < m; i++)
                t[i] = rnd_char("ab");
            for (i = 0; i < n; i++)
                s[i] = rnd_char("ab");
        } else {
            for (i = 0; i < m - 100; i++)
                t[i] = (char)perm[rnd(1, 25)];
            memset(t + m - 100, perm[0], 100);
            memset(s, perm[0], 100);
        }
    }
    s[n] = '\0';
    t[m] = '\0';
    put_string(s);
    put_string(t);
    free(t);
}
