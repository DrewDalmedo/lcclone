/* A string that uses every letter of a chosen set at least once, the rest
 * of it drawn from that set, in random order. Small cases are 1 to 10
 * letters from a set of 1 to 4 letters (a and z among the choices), so
 * letters repeat, near and far apart. Full-size ones are 100 letters, and
 * the seed picks the set: 1 to 26 random letters, a single letter, or the
 * whole alphabet. */
static void fill(char *s, int n, const int *set, int m)
{
    int a[100], i, k;

    for (i = 0; i < n; i++) {
        k = i < m ? i : (int)rnd(0, m - 1);
        a[i] = set[k];
    }
    rnd_shuffle(a, n);
    for (i = 0; i < n; i++)
        s[i] = (char)a[i];
    s[n] = '\0';
}

void gen(int small)
{
    static const char *pools[] = {"z", "az", "xyz", "abyz", "kmpq"};
    char s[101];
    int set[26], n, m, i, shape;
    const char *pool;

    if (small) {
        pool = pools[rnd(0, 4)];
        m = (int)strlen(pool);
        n = (int)rnd(1, 10);
        for (i = 0; i < m; i++)
            set[i] = pool[i];
        rnd_shuffle(set, m);
        if (m > n)
            m = n;
        fill(s, n, set, m);
    } else {
        n = 100;
        for (i = 0; i < 26; i++)
            set[i] = 'a' + i;
        rnd_shuffle(set, 26);
        shape = (int)rnd(0, 3);
        if (shape <= 1)
            m = (int)rnd(1, 26);
        else if (shape == 2)
            m = 1;
        else
            m = 26;
        fill(s, n, set, m);
    }
    put_string(s);
}
