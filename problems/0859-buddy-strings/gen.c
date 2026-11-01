/* Small cases are 1 to 8 letters from "ab", "abc" or eight distinct
 * letters, so that repeats (and words with none) are common. goal is s
 * after one swap, s itself, s after two swaps, s with one letter changed,
 * a word of another length, or random. Full-size cases are 2 * 10^4
 * random letters, in a shape the seed picks: a swap of the first and last
 * letters (true); three letters moved round at far-apart places (false,
 * though the letter counts agree); two differences that don't cross, the
 * second at the end (false); goal equal to s (true); goal one letter
 * longer or shorter, by cutting the last letter off s or off goal
 * (false); or one difference at the end (false). */
static const char *const pools[] = {"ab", "abc", "abcdefgh"};

static void swap_at(char *t, int i, int j)
{
    char c = t[i];

    t[i] = t[j];
    t[j] = c;
}

static void random_swap(char *t, int n)
{
    int i, j;

    if (n < 2)
        return;
    i = (int)rnd(0, n - 1);
    do
        j = (int)rnd(0, n - 1);
    while (j == i);
    swap_at(t, i, j);
}

static void small_case(char *s, char *goal)
{
    const char *pool = pools[rnd(0, 2)];
    int n = (int)rnd(1, 8), shape = (int)rnd(0, 9), m, i;
    char c;

    for (i = 0; i < n; i++)
        s[i] = rnd_char(pool);
    s[n] = '\0';
    memcpy(goal, s, (size_t)n + 1);
    if (shape <= 3) {
        random_swap(goal, n);
    } else if (shape == 5) {
        random_swap(goal, n);
        random_swap(goal, n);
    } else if (shape == 6) {
        c = rnd_char(pool);
        goal[rnd(0, n - 1)] = c;
    } else if (shape == 7) {
        do
            m = (int)rnd(1, 8);
        while (m == n);
        for (i = 0; i < m; i++) {
            /* mostly s's letters, so that one word may begin the other */
            c = rnd_char(pool);
            goal[i] = i < n && rnd_chance(70) ? s[i] : c;
        }
        goal[m] = '\0';
    } else if (shape == 8) {
        for (i = 0; i < n; i++)
            goal[i] = rnd_char(pool);
    }
}

static void large_case(char *s, char *goal)
{
    int n = 20000, shape = (int)(gen_seed() % 6), i;
    int p = (int)rnd(0, n / 3), q = (int)rnd(n / 3 + 1, 2 * n / 3);

    for (i = 0; i < n; i++)
        s[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
    s[n] = '\0';
    memcpy(goal, s, (size_t)n + 1);
    if (shape == 0) {
        s[0] = 'p';
        s[n - 1] = 'q';
        memcpy(goal, s, (size_t)n + 1);
        swap_at(goal, 0, n - 1);
    } else if (shape == 1) {
        s[p] = 'x';
        s[q] = 'y';
        s[n - 1] = 'z';
        memcpy(goal, s, (size_t)n + 1);
        goal[p] = 'y';
        goal[q] = 'z';
        goal[n - 1] = 'x';
    } else if (shape == 2) {
        /* goal[p] == s[n - 1], but goal[n - 1] != s[p] */
        s[p] = 'm';
        s[n - 1] = 'n';
        memcpy(goal, s, (size_t)n + 1);
        goal[p] = 'n';
        goal[n - 1] = 'o';
    } else if (shape == 4) {
        /* both lengths stay within 2 * 10^4 */
        if (rnd_chance(50))
            s[n - 1] = '\0';        /* goal is s and one letter more */
        else
            goal[n - 1] = '\0';     /* goal is s less its last letter */
    } else if (shape == 5) {
        goal[n - 1] = s[n - 1] == 'z' ? 'a' : (char)(s[n - 1] + 1);
    }
}

void gen(int small)
{
    char *s = malloc(20002), *goal = malloc(20002);

    if (small)
        small_case(s, goal);
    else
        large_case(s, goal);
    put_string(s);
    put_string(goal);
    free(s);
    free(goal);
}
