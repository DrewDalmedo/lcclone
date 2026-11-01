/* A results sheet and a set of friends. The seed picks a shape: a random
 * sheet; the sheet in bib order or in reverse bib order; friends who are
 * the last runners to finish; or friends who finish in decreasing bib
 * order. Small cases have 1 to 8 runners. Full-size ones have 100, mostly
 * with 8 friends, and half of the time bib 100 among them. */
void gen(int small)
{
    int order[100], pick[100], friends[8], place[8];
    int n, k, shape, i, j, tmp, m;

    n = small ? (int)rnd(1, 8) : 100;
    if (small)
        k = (int)rnd(1, n < 8 ? n : 8);
    else
        k = rnd_chance(70) ? 8 : (int)rnd(1, 7);
    shape = (int)rnd(0, 3);
    for (i = 0; i < n; i++)
        order[i] = i + 1;
    if (shape != 1)
        rnd_shuffle(order, n);
    else if (rnd_chance(50))
        for (i = 0; i < n / 2; i++) {
            tmp = order[i];
            order[i] = order[n - 1 - i];
            order[n - 1 - i] = tmp;
        }

    if (shape == 2) {
        for (i = 0; i < k; i++)
            friends[i] = order[n - k + i];
    } else {
        for (i = 0; i < n; i++)
            pick[i] = i + 1;
        rnd_shuffle(pick, n);
        for (i = 0; i < k; i++)
            friends[i] = pick[i];
        if (!small && rnd_chance(50)) {
            for (i = 0; i < k && friends[i] != n; i++)
                ;
            if (i == k)
                friends[0] = n;
        }
    }
    /* friends goes in increasing order (insertion sort). */
    for (i = 1; i < k; i++)
        for (j = i; j > 0 && friends[j - 1] > friends[j]; j--) {
            tmp = friends[j];
            friends[j] = friends[j - 1];
            friends[j - 1] = tmp;
        }
    if (shape == 3) {
        /* Hand the friends' places on the sheet out again, the largest
         * bib number first. */
        m = 0;
        for (i = 0; i < n; i++)
            for (j = 0; j < k; j++)
                if (order[i] == friends[j])
                    place[m++] = i;
        for (i = 0; i < k; i++)
            order[place[i]] = friends[k - 1 - i];
    }
    put_ints(order, n);
    put_ints(friends, k);
}
