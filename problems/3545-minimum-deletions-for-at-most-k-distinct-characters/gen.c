/* Small cases: a few characters over a few letters. Full-size cases: 16
 * characters, either 16 different letters (the largest answers) or
 * letters from a pool whose size the seed picks. k is mostly below the
 * number d of different letters in s, so that some must go, and now and
 * then d or more, up to 16. */
void gen(int small)
{
    static const char abc[] = "abcdefghijklmnopqrstuvwxyz";
    char s[17], pool[27];
    int letters[26], seen[26] = {0};
    int n, k, m, d = 0, i, shape;

    if (small) {
        n = (int)rnd(1, 8);
        m = (int)rnd(1, 6);
    } else {
        n = 16;
        shape = (int)rnd(0, 2);
        m = shape == 0 ? 26 : shape == 1 ? (int)rnd(2, 8) : (int)rnd(9, 25);
    }
    /* the pool: m letters picked at random from the alphabet */
    for (i = 0; i < 26; i++)
        letters[i] = i;
    rnd_shuffle(letters, 26);
    for (i = 0; i < m; i++)
        pool[i] = abc[letters[i]];
    pool[m] = '\0';
    if (!small && m == 26) {
        /* 16 different letters, in a random order */
        for (i = 0; i < n; i++)
            s[i] = pool[i];
    } else {
        for (i = 0; i < n; i++)
            s[i] = rnd_char(pool);
    }
    s[n] = '\0';
    for (i = 0; i < n; i++)
        if (!seen[s[i] - 'a']++)
            d++;
    if (rnd_chance(small ? 25 : 15))
        k = (int)rnd(d, 16);
    else
        k = (int)rnd(1, d > 1 ? d - 1 : 1);
    put_string(s);
    put_int(k);
}
