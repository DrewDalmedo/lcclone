/* Two hands of cards. Each number of a pool goes to the first hand only,
 * the second only, or both, with a chance of "both" the seed picks (none,
 * some, or all of them shared), and each hand draws its cards from the
 * numbers it was given, so numbers repeat within a hand.
 * Small cases: 1 to 6 cards per hand, from a pool of 1 to 6 numbers taken
 * from 1, 2, 3, 99, 100 and one random number (so the ends of the range
 * come up often). Full-size cases: the pool is 1 to 100; both hands hold
 * 100 cards most of the time, else 100 and 1, 1 and 100, or random sizes;
 * a hand is sometimes 100 copies of one number. */
void gen(int small)
{
    int pool[100], owner[100], set1[100], set2[100];
    int psize, both, n1 = 0, n2 = 0, n, m, i, x, size, one;
    int *a, *b;

    if (small) {
        int cand[6] = {1, 2, 3, 99, 100, 0};

        cand[5] = (int)rnd(4, 98);
        rnd_shuffle(cand, 6);
        psize = (int)rnd(1, 6);
        for (i = 0; i < psize; i++)
            pool[i] = cand[i];
        n = (int)rnd(1, 6);
        m = (int)rnd(1, 6);
    } else {
        psize = 100;
        for (i = 0; i < 100; i++)
            pool[i] = i + 1;
        x = (int)rnd(0, 9);
        if (x < 6) {
            n = 100;
            m = 100;
        } else if (x == 6) {
            n = 100;
            m = 1;
        } else if (x == 7) {
            n = 1;
            m = 100;
        } else {
            n = (int)rnd(1, 100);
            m = (int)rnd(1, 100);
        }
    }
    x = (int)rnd(0, 4);
    both = x == 0 ? 0 : x == 1 ? 100 : (int)rnd(1, 99);
    for (i = 0; i < psize; i++) {
        if (rnd_chance(both))
            owner[i] = 3;
        else if (rnd_chance(50))
            owner[i] = 1;
        else
            owner[i] = 2;
        if (owner[i] & 1)
            set1[n1++] = pool[i];
        if (owner[i] & 2)
            set2[n2++] = pool[i];
    }
    if (n1 == 0)
        set1[n1++] = pool[0];
    if (n2 == 0)
        set2[n2++] = pool[psize - 1];
    a = malloc((size_t)n * sizeof *a);
    b = malloc((size_t)m * sizeof *b);
    size = n1;
    if (!small && rnd_chance(15))
        size = 1;
    one = (int)rnd(0, n1 - 1);
    for (i = 0; i < n; i++)
        a[i] = size == 1 ? set1[one] : set1[rnd(0, n1 - 1)];
    size = n2;
    if (!small && rnd_chance(15))
        size = 1;
    one = (int)rnd(0, n2 - 1);
    for (i = 0; i < m; i++)
        b[i] = size == 1 ? set2[one] : set2[rnd(0, n2 - 1)];
    put_ints(a, n);
    put_ints(b, m);
    free(a);
    free(b);
}
