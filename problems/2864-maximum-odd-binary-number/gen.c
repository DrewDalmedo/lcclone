/* Small cases: n is 1 to 12. Full-size ones: n is 100 or, now and then,
 * less. The seed picks how many characters are '1' (a single one, all of
 * them, or mostly a random share), and where they go (mostly scattered at
 * random, else all at the front or all at the back). There is always at
 * least one '1'. */
void gen(int small)
{
    char s[101];
    int pos[100];
    int n, ones, how, where, i;

    n = small ? (int)rnd(1, 12) : (rnd_chance(70) ? 100 : (int)rnd(1, 99));
    how = (int)rnd(0, 9);
    if (how == 0)
        ones = 1;
    else if (how == 1)
        ones = n;
    else
        ones = (int)rnd(1, n);
    where = (int)rnd(0, 3);
    for (i = 0; i < n; i++)
        pos[i] = i;
    if (where <= 1)
        rnd_shuffle(pos, n);
    for (i = 0; i < n; i++)
        s[i] = '0';
    for (i = 0; i < ones; i++)
        s[where == 3 ? n - 1 - pos[i] : pos[i]] = '1';
    s[n] = '\0';
    put_string(s);
}
