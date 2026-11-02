/* Small cases: 3 to 12 letters drawn from three or four letters, drawn
 * again until some letter has an odd count and some letter a non-zero even
 * one, so that ties and negative answers are common. Full-size ones are
 * 100 letters with counts picked by the seed: one letter far more common
 * than the rest, with an odd or an even count (answers near 95 or -97);
 * all 26 letters; or random counts over a random set of letters. */
static int valid(const int *count)
{
    int odd = 0, even = 0, i;

    for (i = 0; i < 26; i++) {
        if (count[i] > 0 && count[i] % 2 == 1)
            odd = 1;
        if (count[i] > 0 && count[i] % 2 == 0)
            even = 1;
    }
    return odd && even;
}

static void shuffle(char *s, int n)
{
    int i, j;
    char t;

    for (i = n - 1; i > 0; i--) {
        j = (int)rnd(0, i);
        t = s[i];
        s[i] = s[j];
        s[j] = t;
    }
}

void gen(int small)
{
    static const char *az = "abcdefghijklmnopqrstuvwxyz";
    char s[101];
    int count[26], letters[26], n, i, k, c, shape, m, big;
    const char *pool;

    if (small) {
        pool = rnd_chance(50) ? "abc" : "wxyz";
        do {
            memset(count, 0, sizeof count);
            n = (int)rnd(3, 12);
            for (i = 0; i < n; i++) {
                s[i] = rnd_char(pool);
                count[s[i] - 'a']++;
            }
        } while (!valid(count));
        s[n] = '\0';
        put_string(s);
        return;
    }
    for (i = 0; i < 26; i++)
        letters[i] = i;
    rnd_shuffle(letters, 26);
    shape = (int)rnd(0, 3);
    do {
        memset(count, 0, sizeof count);
        n = 0;
        if (shape == 0 || shape == 1) {
            /* One letter with nearly everything: odd (shape 0) or even. */
            m = (int)rnd(2, 6);
            for (i = 1; i < m; i++) {
                c = (int)rnd(1, 2);
                count[letters[i]] = c;
                n += c;
            }
            big = 100 - n;
            if ((big % 2 == 1) != (shape == 0)) {
                big--;
                count[letters[1]]++;
            }
            count[letters[0]] = big;
            n = 100;
        } else if (shape == 2) {
            for (i = 0; i < 26; i++)
                count[i] = 1;
            for (k = 26; k < 100; k++) {
                c = (int)rnd(0, 25);
                count[c]++;
            }
            n = 100;
        } else {
            m = (int)rnd(2, 26);
            for (i = 0; i < m; i++)
                count[letters[i]] = 1;
            for (k = m; k < 100; k++) {
                c = (int)rnd(0, m - 1);
                count[letters[c]]++;
            }
            n = 100;
        }
    } while (!valid(count));
    n = 0;
    for (i = 0; i < 26; i++)
        for (k = 0; k < count[i]; k++)
            s[n++] = az[i];
    shuffle(s, n);
    s[n] = '\0';
    put_string(s);
}
