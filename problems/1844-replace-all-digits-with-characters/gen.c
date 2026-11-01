/* Letters at the even indices, and after each a digit that keeps the
 * letter it stands for within a to z. Small cases have 1 to 8 characters,
 * with letters often from the end of the alphabet (q, y, z) or a, and
 * digits often 0 or the largest allowed. Full-size ones have 100 or 99
 * characters (ending in a digit or a letter), as the seed picks. */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 8) : (int)rnd(99, 100), i, top, d;

    for (i = 0; i < n; i++) {
        if (i % 2 == 0) {
            if (small && rnd_chance(50))
                s[i] = rnd_char("aqyz");
            else
                s[i] = (char)rnd('a', 'z');
            continue;
        }
        top = 'z' - s[i - 1];
        if (top > 9)
            top = 9;
        if (small && rnd_chance(50))
            d = rnd_chance(50) ? 0 : top;
        else
            d = (int)rnd(0, top);
        s[i] = (char)('0' + d);
    }
    s[n] = '\0';
    put_string(s);
}
