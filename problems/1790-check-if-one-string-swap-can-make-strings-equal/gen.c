/* Small cases are 1 to 8 letters from "xy", "pqr" or eight distinct
 * letters, so that repeats (and strings with none) are common. s2 is s1
 * itself, s1 after one exchange (mostly of two different letters), after
 * two, with one letter changed, with three letters moved round, with two
 * letters changed where only one crosses, or random. Full-size cases are
 * 100 random letters, in a shape the seed picks: the first and last
 * letters exchanged (true); every position different (false); three
 * letters moved round at far-apart places, the last at the end (false,
 * though the letter counts agree); s2 equal to s1 (true); two separate
 * exchanges, the second at the end (false); or two differences where
 * only one letter crosses (false). */
static const char *const pools[] = {"xy", "pqr", "klmnoprs"};

static void swap_at(char *t, int i, int j)
{
    char c = t[i];

    t[i] = t[j];
    t[j] = c;
}

/* Two different positions of a string of n >= 2 letters. */
static void two_places(int n, int *i, int *j)
{
    *i = (int)rnd(0, n - 1);
    do
        *j = (int)rnd(0, n - 1);
    while (*j == *i);
}

static void small_case(char *s1, char *s2)
{
    int n = (int)rnd(1, 8), shape = (int)rnd(0, 9), i, j, k;
    const char *pool = pools[rnd(0, 2)];
    char c;

    for (k = 0; k < n; k++)
        s1[k] = rnd_char(pool);
    s1[n] = '\0';
    memcpy(s2, s1, (size_t)n + 1);
    if (shape == 0) {
        /* s2 stays equal to s1 */
    } else if (n == 1) {
        s2[0] = rnd_char(pool);
    } else if (shape <= 4) {
        /* a few tries at two places holding different letters */
        k = 0;
        do
            two_places(n, &i, &j);
        while (s1[i] == s1[j] && ++k < 4);
        swap_at(s2, i, j);
    } else if (shape == 5) {
        two_places(n, &i, &j);
        swap_at(s2, i, j);
        two_places(n, &i, &j);
        swap_at(s2, i, j);
    } else if (shape == 6) {
        c = rnd_char(pool);
        s2[rnd(0, n - 1)] = c;
    } else if (shape == 7 && n >= 3) {
        two_places(n, &i, &j);
        do
            k = (int)rnd(0, n - 1);
        while (k == i || k == j);
        s2[i] = s1[j];
        s2[j] = s1[k];
        s2[k] = s1[i];
    } else if (shape == 8) {
        two_places(n, &i, &j);
        c = rnd_char(pool);
        s2[i] = s1[j];
        s2[j] = c;
    } else {
        for (k = 0; k < n; k++)
            s2[k] = rnd_char(pool);
    }
}

static void large_case(char *s1, char *s2)
{
    int n = 100, shape = (int)(gen_seed() % 6), k;
    /* q > p + 1, so that s1[p + 1] (shape 4) never lands on s1[q] */
    int p = (int)rnd(0, n / 3), q = (int)rnd(n / 3 + 2, 2 * n / 3);

    for (k = 0; k < n; k++)
        s1[k] = rnd_char("abcdefghijklmnopqrstuvwxyz");
    s1[n] = '\0';
    if (shape == 0) {
        s1[0] = 'p';
        s1[n - 1] = 'q';
    } else if (shape == 2) {
        s1[p] = 'x';
        s1[q] = 'y';
        s1[n - 1] = 'z';
    } else if (shape == 4) {
        s1[p] = 'f';
        s1[p + 1] = 'g';
        s1[q] = 'h';
        s1[n - 1] = 'i';
    } else if (shape == 5) {
        s1[p] = 'm';
        s1[n - 1] = 'n';
    }
    memcpy(s2, s1, (size_t)n + 1);
    if (shape == 0) {
        swap_at(s2, 0, n - 1);
    } else if (shape == 1) {
        for (k = 0; k < n; k++)
            s2[k] = s1[k] == 'z' ? 'a' : (char)(s1[k] + 1);
    } else if (shape == 2) {
        s2[p] = 'y';
        s2[q] = 'z';
        s2[n - 1] = 'x';
    } else if (shape == 4) {
        swap_at(s2, p, p + 1);
        swap_at(s2, q, n - 1);
    } else if (shape == 5) {
        /* s2[p] == s1[n - 1], but s2[n - 1] != s1[p] */
        s2[p] = 'n';
        s2[n - 1] = 'o';
    }
}

void gen(int small)
{
    char s1[101], s2[101];

    if (small)
        small_case(s1, s2);
    else
        large_case(s1, s2);
    put_string(s1);
    put_string(s2);
}
