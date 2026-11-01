/* Lowercase letters from an alphabet of a size the seed picks: one letter,
 * two, three or four (so a third letter turns up at varied places), or all
 * 26. Small cases have up to 12 letters; full-size ones have 100. */
void gen(int small)
{
    static const char *alphabets[] = {"a", "xy", "pqr", "dcba",
                                      "abcdefghijklmnopqrstuvwxyz"};
    char s[101], late;
    int n, which, i;

    n = small ? (int)rnd(1, 12) : 100;
    which = (int)rnd(0, 4);
    for (i = 0; i < n; i++)
        s[i] = rnd_char(alphabets[which]);
    if (!small && which == 1 && rnd_chance(50)) {
        /* two letters, then a third one late in the string */
        late = rnd_char("mn");
        s[rnd(n / 2, n - 1)] = late;
    }
    s[n] = '\0';
    put_string(s);
}
