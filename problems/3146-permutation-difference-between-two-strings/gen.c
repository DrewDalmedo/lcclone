/* s is n distinct letters in a random order; t rearranges them in a shape
 * the seed picks: now and then unchanged, else reversed, rotated, with two
 * letters swapped, or shuffled. Small cases have 1 to 7 letters; full-size
 * ones all 26, or nearly. */
void gen(int small)
{
    int letters[26], order[26], i, n, k, a, b, tmp, shape;
    char s[27], t[27];

    for (i = 0; i < 26; i++)
        letters[i] = i;
    rnd_shuffle(letters, 26);
    n = small ? (int)rnd(1, 7) : rnd_chance(60) ? 26 : (int)rnd(20, 25);
    for (i = 0; i < n; i++) {
        s[i] = (char)('a' + letters[i]);
        order[i] = i;
    }
    s[n] = '\0';
    shape = (int)rnd(0, 8);
    if (shape == 1 || shape == 2) {
        for (i = 0; i < n; i++)
            order[i] = n - 1 - i;
    } else if (shape == 3 || shape == 4) {
        k = n > 1 ? (int)rnd(1, n - 1) : 0;
        for (i = 0; i < n; i++)
            order[i] = (i + k) % n;
    } else if (shape == 5 || shape == 6) {
        a = (int)rnd(0, n - 1);
        b = n > 1 ? (int)rnd(0, n - 2) : 0;
        if (n > 1 && b >= a)
            b++;
        tmp = order[a];
        order[a] = order[b];
        order[b] = tmp;
    } else if (shape >= 7) {
        rnd_shuffle(order, n);
    }
    for (i = 0; i < n; i++)
        t[i] = s[order[i]];
    t[n] = '\0';
    put_string(s);
    put_string(t);
}
