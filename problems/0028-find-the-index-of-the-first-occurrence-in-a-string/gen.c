/* Small cases: a haystack of 1 to 10 letters from a, b (and sometimes c),
 * and a needle of 1 to 6 letters: random, or a piece of the haystack,
 * maybe with one letter changed, so that matches, near misses and needles
 * longer than the haystack all come up. Full-size ones pick the shape from
 * the seed, most of them slow for checking every index: a run of a with a
 * needle a...ab of about half its length, or a repeating pattern with a
 * needle that follows it but ends in z (both found at the very end, or not
 * at all); random text with the needle taken from its end; or a needle of
 * 10^4 letters that starts with a shorter haystack. */
static void fill(char *s, int len, const char *letters)
{
    int i;

    for (i = 0; i < len; i++)
        s[i] = rnd_char(letters);
    s[len] = '\0';
}

void gen(int small)
{
    int maxlen = small ? 10 : 10000, n, m, i, start, shape;
    char *h = malloc((size_t)maxlen + 1), *nd = malloc((size_t)maxlen + 1);

    if (small) {
        n = (int)rnd(1, 10);
        fill(h, n, rnd_chance(70) ? "ab" : "abc");
        if (rnd_chance(60)) {
            start = (int)rnd(0, n - 1);
            m = (int)rnd(1, n - start + (rnd_chance(20) ? 2 : 0));
            if (m > 6)
                m = 6;
            for (i = 0; i < m; i++)
                nd[i] = start + i < n ? h[start + i] : rnd_char("ab");
            nd[m] = '\0';
            if (rnd_chance(30)) {
                char c = rnd_char("abc");

                nd[rnd(0, m - 1)] = c;
            }
        } else {
            m = (int)rnd(1, 6);
            fill(nd, m, "ab");
        }
    } else {
        shape = (int)rnd(0, 3);
        n = maxlen;
        if (shape == 0) {
            m = (int)rnd(4000, 6000);
            memset(h, 'a', (size_t)n);
            h[n] = '\0';
            if (rnd_chance(50))
                h[n - 1] = 'b';
            memset(nd, 'a', (size_t)m - 1);
            nd[m - 1] = 'b';
            nd[m] = '\0';
        } else if (shape == 1) {
            int period = (int)rnd(2, 5);

            m = (int)rnd(1000, 5000);
            for (i = 0; i < n; i++)
                h[i] = (char)('a' + i % period);
            h[n] = '\0';
            if (rnd_chance(50))
                h[n - 1] = 'z';
            for (i = 0; i < m; i++)
                nd[i] = (char)('a' + (n - m + i) % period);
            nd[m - 1] = 'z';
            nd[m] = '\0';
        } else if (shape == 2) {
            fill(h, n, rnd_chance(50) ? "ab" : "abcdefghijklmnopqrstuvwxyz");
            m = (int)rnd(1, 50);
            memcpy(nd, h + n - m, (size_t)m);
            nd[m] = '\0';
        } else {
            n = (int)rnd(1, 9999);
            m = maxlen;
            fill(nd, m, "ab");
            memcpy(h, nd, (size_t)n);
            h[n] = '\0';
        }
    }
    put_string(h);
    put_string(nd);
    free(h);
    free(nd);
}
