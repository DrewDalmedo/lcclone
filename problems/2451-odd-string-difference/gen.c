/* The common steps come from a random shape word whose letters span a
 * random range; every other word is the shape moved up or down the
 * alphabet by a random amount that keeps it within a..z (so equal words
 * come up too). The odd word starts as one more such copy, then one
 * letter is changed (the last, the first or any), or it is reversed,
 * shifted with wrap-around past z, or shuffled; if that happens to keep
 * the steps, its last letter is changed instead. Small cases are 3 to 6
 * words of 2 to 4 letters, the odd one often at index 0 or 1. Full-size
 * ones are 100 words of 20 letters whose odd word differs only in its
 * last step, placed last, first or anywhere, as the seed picks. */
static char text[100][21];

/* Writes the shape moved by d into w. */
static void place(char *w, const int *shape, int n, int d)
{
    int i;

    for (i = 0; i < n; i++)
        w[i] = (char)('a' + shape[i] + d);
    w[n] = '\0';
}

static int same_steps(const char *a, const char *b, int n)
{
    int j;

    for (j = 0; j + 1 < n; j++)
        if (a[j + 1] - a[j] != b[j + 1] - b[j])
            return 0;
    return 1;
}

/* Replaces w[i] with a different letter. */
static void change(char *w, int i)
{
    int t = (int)rnd(0, 24);

    w[i] = (char)('a' + (t >= w[i] - 'a' ? t + 1 : t));
}

void gen(int small)
{
    char base[21], odd_word[21], *words[100];
    int shape[20], perm[20];
    int m, n, span, lo, hi, i, d, odd, how, t;

    if (small) {
        m = (int)rnd(3, 6);
        n = (int)rnd(2, 4);
        span = rnd_chance(25) ? (int)rnd(0, 25) : (int)rnd(0, 3);
    } else {
        m = 100;
        n = 20;
        span = (int)rnd(0, 25);
    }
    lo = 25;
    hi = 0;
    for (i = 0; i < n; i++) {
        shape[i] = (int)rnd(0, span);
        if (shape[i] < lo)
            lo = shape[i];
        if (shape[i] > hi)
            hi = shape[i];
    }
    for (i = 0; i < m; i++) {
        d = (int)rnd(-lo, 25 - hi);
        place(text[i], shape, n, d);
    }
    if (small) {
        t = (int)rnd(0, 3);
        odd = t < 2 ? t : (int)rnd(0, m - 1);
        how = (int)rnd(0, 5);
    } else {
        t = (int)(gen_seed() % 3);
        odd = t == 0 ? m - 1 : t == 1 ? 0 : (int)rnd(0, m - 1);
        how = 0;
    }
    d = (int)rnd(-lo, 25 - hi);
    place(base, shape, n, d);
    memcpy(odd_word, base, (size_t)n + 1);
    switch (how) {
    case 0:
        change(odd_word, n - 1);
        break;
    case 1:
        change(odd_word, 0);
        break;
    case 2:
        t = (int)rnd(0, n - 1);
        change(odd_word, t);
        break;
    case 3:
        for (i = 0; i < n; i++)
            odd_word[i] = base[n - 1 - i];
        break;
    case 4:
        t = (int)rnd(1, 25);
        for (i = 0; i < n; i++)
            odd_word[i] = (char)('a' + (base[i] - 'a' + t) % 26);
        break;
    default:
        for (i = 0; i < n; i++)
            perm[i] = i;
        rnd_shuffle(perm, n);
        for (i = 0; i < n; i++)
            odd_word[i] = base[perm[i]];
        break;
    }
    if (same_steps(odd_word, base, n)) {
        memcpy(odd_word, base, (size_t)n + 1);
        change(odd_word, n - 1);
    }
    memcpy(text[odd], odd_word, (size_t)n + 1);
    for (i = 0; i < m; i++)
        words[i] = text[i];
    put_strings(words, m);
}
