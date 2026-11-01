/* An attendance log. Small cases have up to 12 days, each an 'A' or an 'L'
 * with chances the seed picks, so that logs with no, one or two absences,
 * and late runs of two and three, all come up often. Full-size ones have
 * 1000 days (even seeds) or a random number up to 1000, in a shape the seed
 * picks: late days in pairs with at most one absence, so the whole log
 * passes and must be read to the end; the same with a second absence on
 * the last day, or with three late days only at the very end; all 'P' or
 * all 'L'; or random. */
static void put_log(char *s, int n)
{
    s[n] = '\0';
    put_string(s);
}

void gen(int small)
{
    static const int achance[] = {0, 5, 10, 20, 40};
    static const int lchance[] = {20, 40, 60, 80};
    static char s[1001];
    int n, pa, pl, i, shape, r;

    if (small) {
        n = (int)rnd(1, 12);
        pa = achance[rnd(0, 4)];
        pl = lchance[rnd(0, 3)];
        for (i = 0; i < n; i++) {
            r = (int)rnd(0, 99);
            s[i] = r < pa ? 'A' : r < pa + pl ? 'L' : 'P';
        }
        put_log(s, n);
        return;
    }
    n = gen_seed() % 2 == 0 ? 1000 : (int)rnd(4, 1000);
    shape = (int)rnd(0, 4);
    if (shape == 3) {
        memset(s, rnd_chance(50) ? 'P' : 'L', (size_t)n);
        put_log(s, n);
        return;
    }
    if (shape == 4) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("ALPPPPLL");
        put_log(s, n);
        return;
    }
    /* late days in runs of one or two, each run followed by a 'P' */
    for (i = 0; i < n;) {
        r = (int)rnd(1, 2);
        while (r-- > 0 && i < n)
            s[i++] = 'L';
        if (i < n)
            s[i++] = 'P';
    }
    if (rnd_chance(70))
        s[rnd(0, n - 1)] = 'A';
    if (shape == 1) {
        /* one absence before the last day, and another on it */
        for (i = 0; i < n - 1; i++)
            if (s[i] == 'A')
                break;
        if (i == n - 1)
            s[0] = 'A';
        s[n - 1] = 'A';
    } else if (shape == 2) {
        s[n - 4] = 'P';
        s[n - 3] = s[n - 2] = s[n - 1] = 'L';
    }
    put_log(s, n);
}
