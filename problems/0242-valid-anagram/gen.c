/* t is usually s shuffled, and then often spoiled: one letter changed,
 * one letter added or removed, or two letters moved one step apart in the
 * alphabet in opposite directions (which keeps the sum of the character
 * codes). Otherwise t is unrelated to s. Small cases use a few letters
 * from a tiny alphabet; full-size ones are 5 * 10^4 letters long, in a
 * shape the seed picks. */
static void shuffle(char *a, int n)
{
    int i, k;
    char c;

    for (i = n - 1; i > 0; i--) {
        k = (int)rnd(0, i);
        c = a[i];
        a[i] = a[k];
        a[k] = c;
    }
}

/* Changes t[0..m) so that its code sum stays the same: one letter up a
 * step and another down a step, when there are two that can move. */
static void same_sum(char *t, int m)
{
    int i, k;

    if (m < 2)
        return;
    i = (int)rnd(0, m - 1);
    k = (int)rnd(0, m - 2);
    if (k >= i)
        k++;
    if (t[i] < 'z' && t[k] > 'a') {
        t[i]++;
        t[k]--;
    }
}

void gen(int small)
{
    static const char *alphabets[] = {
        "ab", "abc", "az", "abcdefghijklmnopqrstuvwxyz"
    };
    const char *abc;
    int max = small ? 8 : 50000, n, m, i, shape;
    char *s = malloc((size_t)max + 2), *t = malloc((size_t)max + 2);

    if (small) {
        abc = alphabets[rnd(0, 3)];
        shape = (int)rnd(0, 9);
    } else {
        abc = alphabets[3];
        shape = (int)(gen_seed() % 5) * 2; /* 0, 2, 4, 6 or 8 */
    }
    n = small ? (int)rnd(1, max) : (shape == 6 ? max - 1 : max);
    for (i = 0; i < n; i++)
        s[i] = rnd_char(abc);
    memcpy(t, s, (size_t)n);
    m = n;
    shuffle(t, m);
    if (shape <= 3) {
        /* an anagram */
    } else if (shape == 4) {
        char c = rnd_char(abc);

        t[small ? rnd(0, m - 1) : m - 1] = c;
    } else if (shape == 5 || shape == 6) {
        if (shape == 6 || m == 1 || rnd_chance(50))
            t[m++] = rnd_char(abc);
        else
            m--;
    } else if (shape <= 8) {
        same_sum(t, m);
    } else {
        m = (int)rnd(1, max);
        for (i = 0; i < m; i++)
            t[i] = rnd_char(abc);
    }
    s[n] = '\0';
    t[m] = '\0';
    put_string(s);
    put_string(t);
    free(s);
    free(t);
}
