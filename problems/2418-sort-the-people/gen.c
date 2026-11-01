/* Names drawn from a small pool, so that the same name (and the same
 * letters in another case) comes up for several singers, and distinct
 * heights. The seed picks the order of the heights: random, shortest
 * first or tallest first. Small cases have 1 to 8 singers with heights up
 * to 12; full-size ones have 1000 singers, names of 1 to 20 letters and
 * heights up to 10^5. Either way the tallest is sometimes raised to 10^5. */
static int asc(const void *x, const void *y)
{
    int a = *(const int *)x, b = *(const int *)y;

    return (a > b) - (a < b);
}

void gen(int small)
{
    static const char *tiny[] = {"a", "A", "ab", "Ab", "AB", "Zed", "zed",
                                 "Q", "Abcdefghijklmnopqrst"};
    static char vocab[1000][21];
    static char seen[100001];
    int n = small ? (int)rnd(1, 8) : 1000;
    int top = small ? 12 : 100000;
    int order = (int)rnd(0, 2), pool, i, k, len, h, hi = 0;
    int *heights = malloc((size_t)n * sizeof *heights);
    char **names = malloc((size_t)n * sizeof *names);

    pool = small ? (int)rnd(1, 9) : (int)rnd(1, 1000);
    if (!small)
        for (i = 0; i < pool; i++) {
            len = (int)rnd(1, 20);
            for (k = 0; k < len; k++)
                vocab[i][k] = rnd_char("abcdefghijklmnopqrstuvwxyz"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            vocab[i][len] = '\0';
        }
    for (i = 0; i < n; i++) {
        k = (int)rnd(0, pool - 1);
        names[i] = small ? (char *)tiny[k] : vocab[k];
    }
    memset(seen, 0, sizeof seen);
    for (i = 0; i < n; i++) {
        do
            h = (int)rnd(1, top);
        while (seen[h]);
        seen[h] = 1;
        heights[i] = h;
    }
    if (rnd_chance(small ? 20 : 50)) {
        for (i = 1; i < n; i++)
            if (heights[i] > heights[hi])
                hi = i;
        heights[hi] = 100000;
    }
    if (order > 0) {
        qsort(heights, (size_t)n, sizeof *heights, asc);
        if (order == 2)
            for (i = 0; i < n / 2; i++) {
                h = heights[i];
                heights[i] = heights[n - 1 - i];
                heights[n - 1 - i] = h;
            }
    }
    put_strings(names, n);
    put_ints(heights, n);
    free(heights);
    free(names);
}
