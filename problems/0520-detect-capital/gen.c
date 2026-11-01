/* A word written in one of the three patterns or with random cases; a
 * word in a pattern then usually has the case of one or two letters
 * flipped, at the last index, at index 1 or anywhere. Small cases are 1 to
 * 5 letters, so that every short shape comes up; full-size ones are 100
 * letters, the longest a word can be. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 5) : 100;
    int shape = (int)rnd(0, 3), flips, where, at, i, capital;
    char w[101], c;

    for (i = 0; i < n; i++) {
        c = small ? rnd_char("abqz") : (char)rnd('a', 'z');
        if (shape == 3)
            capital = rnd_chance(50);
        else
            capital = shape == 1 || (shape == 2 && i == 0);
        w[i] = capital ? (char)(c - 'a' + 'A') : c;
    }
    flips = shape == 3 || rnd_chance(25) ? 0 : (int)rnd(1, 2);
    for (i = 0; i < flips; i++) {
        where = (int)rnd(0, 2);
        if (where == 0)
            at = n - 1;
        else if (where == 1)
            at = n > 1 ? 1 : 0;
        else
            at = (int)rnd(0, n - 1);
        w[at] = (char)(w[at] ^ 32);
    }
    w[n] = '\0';
    put_string(w);
}
