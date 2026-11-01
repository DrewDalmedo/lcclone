/* One string of lowercase letters. Small cases have up to 10 letters from
 * a small alphabet the seed picks ("ab", "az", "xyz", ...), so letters
 * repeat and turns at a and z come up often. Full-size cases have 500
 * letters, in a shape the seed picks:
 *   0: letters drawn evenly from a..z;
 *   1: one letter 256 to 500 times (all of s at 500), the rest from
 *      a..z: the many-passes shape, and a count no byte can hold;
 *   2: two to four letters, always a and z among them, with uneven
 *      counts, so they run out at different passes. */
static const char *const tiny[] = {
    "ab", "abc", "az", "amz", "xyz", "abcz", "bdfh",
    "abcdefghijklmnopqrstuvwxyz"
};

void gen(int small)
{
    char s[501];
    int a[500], letter[4], weight[4];
    int n, shape, heavy, count, m, total, r, i, j;
    const char *alpha;

    if (small) {
        n = (int)rnd(1, 10);
        alpha = tiny[rnd(0, 7)];
        for (i = 0; i < n; i++)
            s[i] = rnd_char(alpha);
        s[n] = '\0';
        put_string(s);
        return;
    }
    n = 500;
    shape = (int)rnd(0, 2);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            a[i] = (int)rnd(0, 25);
    } else if (shape == 1) {
        heavy = (int)rnd(0, 25);
        count = (int)rnd(256, 500);
        for (i = 0; i < n; i++)
            a[i] = i < count ? heavy : (int)rnd(0, 25);
    } else {
        m = (int)rnd(2, 4);
        letter[0] = 0;
        letter[1] = 25;
        for (j = 2; j < m; j++)
            letter[j] = (int)rnd(1, 24);
        total = 0;
        for (j = 0; j < m; j++) {
            weight[j] = (int)rnd(1, 100);
            total += weight[j];
        }
        for (i = 0; i < n; i++) {
            r = (int)rnd(0, total - 1);
            for (j = 0; r >= weight[j]; j++)
                r -= weight[j];
            a[i] = letter[j];
        }
    }
    rnd_shuffle(a, n);
    for (i = 0; i < n; i++)
        s[i] = (char)('a' + a[i]);
    s[n] = '\0';
    put_string(s);
}
