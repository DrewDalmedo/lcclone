/* Two lists of distinct digits, each in a random order. Half of the time
 * they are drawn apart, from one shuffle of 1 to 9, so that they share
 * nothing; otherwise each list is drawn on its own, and they may share
 * digits. Small cases favour short lists (1 to 4 digits); full-size ones
 * have 5 to 9 digits in at least one list. */
static void pick(int *out, int n)
{
    int d[9], i;

    for (i = 0; i < 9; i++)
        d[i] = i + 1;
    rnd_shuffle(d, 9);
    for (i = 0; i < n; i++)
        out[i] = d[i];
}

void gen(int small)
{
    int a[9], b[9], d[9], n1, n2, i;

    if (small) {
        n1 = (int)rnd(1, rnd_chance(75) ? 4 : 9);
        n2 = (int)rnd(1, rnd_chance(75) ? 4 : 9);
    } else {
        n1 = (int)rnd(5, 9);
        n2 = (int)rnd(1, 9);
    }
    if (rnd_chance(50)) {
        if (n1 > 8)
            n1 = 8;
        if (n1 + n2 > 9)
            n2 = 9 - n1;
        for (i = 0; i < 9; i++)
            d[i] = i + 1;
        rnd_shuffle(d, 9);
        for (i = 0; i < n1; i++)
            a[i] = d[i];
        for (i = 0; i < n2; i++)
            b[i] = d[n1 + i];
    } else {
        pick(a, n1);
        pick(b, n2);
    }
    if (rnd_chance(50)) {
        put_ints(a, n1);
        put_ints(b, n2);
    } else {
        put_ints(b, n2);
        put_ints(a, n1);
    }
}
