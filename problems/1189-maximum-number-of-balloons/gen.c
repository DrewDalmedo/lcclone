/* Small cases: a few copies of the word with some letters taken out and
 * a few others put in, shuffled, so each of b, a, l, o and n gets to be
 * the one that runs out. Full-size ones are 10^4 letters, with the seed
 * picking the shape: as many copies as fit (1428), one of the five
 * letters missing (answer 0), only the five letters at random, or any
 * letters at random. */
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
    static const char word[] = "balloon", five[] = "balon";
    char *s = malloc(10001), others[6];
    int n = 0, k, i, j, shape, drop, add;
    char missing;

    if (small) {
        k = (int)rnd(0, 3);
        for (i = 0; i < k; i++)
            for (j = 0; j < 7; j++)
                s[n++] = word[j];
        drop = (int)rnd(0, 3);
        for (i = 0; i < drop && n > 0; i++) {
            j = (int)rnd(0, n - 1);
            s[j] = s[--n];
        }
        add = (int)rnd(0, 4);
        if (n + add == 0)
            add = 1;
        for (i = 0; i < add; i++)
            s[n++] = rnd_char("balonbalonxe");
    } else {
        n = 10000;
        shape = (int)rnd(0, 3);
        if (shape == 0) {
            for (i = 0; i < 1428; i++)
                for (j = 0; j < 7; j++)
                    s[7 * i + j] = word[j];
            for (i = 7 * 1428; i < n; i++)
                s[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        } else if (shape == 1) {
            missing = five[rnd(0, 4)];
            for (i = j = 0; five[i]; i++)
                if (five[i] != missing)
                    others[j++] = five[i];
            others[j] = '\0';
            for (i = 0; i < n; i++)
                s[i] = rnd_char(others);
        } else if (shape == 2) {
            for (i = 0; i < n; i++)
                s[i] = rnd_char(five);
        } else {
            for (i = 0; i < n; i++)
                s[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        }
    }
    shuffle(s, n);
    s[n] = '\0';
    put_string(s);
    free(s);
}
