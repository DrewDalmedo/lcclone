/* Widths for the 26 letters and a string s. Widths are made in one of
 * three ways: the same width for every letter; each letter 2 to 10 at
 * random; or each letter one of 2, 4, 5 and 10, which often add up to
 * exactly 100. Small cases have up to 45 letters drawn from a few letters
 * of the alphabet, so rows fill and overflow by small amounts. Full-size
 * ones have 1000 letters from the whole alphabet, and the seed picks the
 * widths: every letter 10 wide (the largest answer, [100,100]), every
 * letter 2 wide (50 letters to a row), or one of the three ways above. */
void gen(int small)
{
    static const int round[] = {2, 4, 5, 10};
    int widths[26], n = small ? (int)rnd(1, 45) : 1000, kind, w, i;
    int used = small ? (int)rnd(1, 4) : 26;
    char letters[26], *s = malloc((size_t)n + 1);

    kind = small ? (int)(gen_seed() % 3) : (int)(gen_seed() % 5);
    w = kind == 3 ? 10 : kind == 4 ? 2 : (int)rnd(2, 10);
    for (i = 0; i < 26; i++) {
        if (kind == 1)
            widths[i] = (int)rnd(2, 10);
        else if (kind == 2)
            widths[i] = round[rnd(0, 3)];
        else
            widths[i] = w;
    }
    for (i = 0; i < used; i++)
        letters[i] = small ? (char)('a' + rnd(0, 25)) : (char)('a' + i);
    for (i = 0; i < n; i++)
        s[i] = letters[rnd(0, used - 1)];
    s[n] = '\0';
    put_ints(widths, 26);
    put_string(s);
    free(s);
}
