/* Letters and special characters, with the share of letters chosen by
 * the seed: none, all, about half, or just one or two of one kind among
 * the other. Small cases have up to 10 characters from a few letters and
 * specials (brackets among them); full-size ones have 100 characters from
 * all 26 letters and all ten specials. */
void gen(int small)
{
    const char *letters = small ? "abc" : "abcdefghijklmnopqrstuvwxyz";
    const char *specials = small ? "!()#" : "!@#$%^&*()";
    char s[101];
    int n, shape, pct, i, is_letter;

    n = small ? (int)rnd(1, 10) : 100;
    shape = (int)rnd(0, 4);
    if (shape == 0)
        pct = 0;
    else if (shape == 1)
        pct = 100;
    else if (shape == 2)
        pct = small ? 15 : 2;
    else if (shape == 3)
        pct = small ? 85 : 98;
    else
        pct = (int)rnd(25, 75);
    for (i = 0; i < n; i++) {
        is_letter = rnd_chance(pct);
        s[i] = is_letter ? rnd_char(letters) : rnd_char(specials);
    }
    s[n] = '\0';
    put_string(s);
}
