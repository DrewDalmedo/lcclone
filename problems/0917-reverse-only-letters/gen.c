/* Small cases are up to 8 characters from a tiny pool (a, b, A, B, Z and
 * the non-letters next to the letter ranges: @ [ ] ^ _ ` plus 1 and !),
 * with a share of letters the seed picks, from none to all, so that
 * strings without letters, with one letter, with only letters and with
 * repeated letters come up often. Full-size cases are 100 characters, in a
 * shape the seed picks: mixed; only letters; letters only at both ends;
 * letters only in the middle, with long runs of non-letters around them;
 * or no letter at all. */
static const char letters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char others[] = "!#$%&'()*+,-./0123456789:;<=>?@[]^_`";

void gen(int small)
{
    static const int percent[] = {0, 25, 50, 80, 100};
    int n = small ? (int)rnd(1, 8) : 100, i, shape, letter;
    char s[101];

    shape = small ? (int)rnd(0, 4) : (int)(gen_seed() % 5);
    for (i = 0; i < n; i++) {
        if (small)
            letter = rnd_chance(percent[shape]);
        else if (shape == 0)
            letter = rnd_chance(50);
        else if (shape == 1)
            letter = 1;
        else if (shape == 2)
            letter = i < 3 || i >= n - 3;
        else if (shape == 3)
            letter = i >= 40 && i < 60;
        else
            letter = 0;
        if (small)
            s[i] = rnd_char(letter ? "abABZ" : "@[]^_`1!");
        else
            s[i] = rnd_char(letter ? letters : others);
    }
    s[n] = '\0';
    put_string(s);
}
