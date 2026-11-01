/* A string of '0' and '1' in one of four shapes, picked at random: fair
 * coin flips, mostly one character with a few of the other, alternating
 * with a few slips, or runs of random lengths. k is usually small (1 or 2
 * in small cases, 1 to 3 in full-size ones), where the constraint cuts the
 * count down; sometimes anything up to the length.
 * Small cases are 1 to 10 characters long, full-size ones 50. */
void gen(int small)
{
    char s[51];
    int n = small ? (int)rnd(1, 10) : 50;
    int shape = (int)rnd(0, 3), i, k, top;
    char c = rnd_char("01");

    for (i = 0; i < n; i++) {
        if (shape == 0) {
            s[i] = rnd_char("01");
        } else if (shape == 1) {
            s[i] = rnd_chance(15) ? (char)('0' + '1' - c) : c;
        } else if (shape == 2) {
            s[i] = (char)((i % 2 == 0) == (c == '0') ? '0' : '1');
            if (rnd_chance(10))
                s[i] = (char)('0' + '1' - s[i]);
        } else {
            if (i > 0 && rnd_chance(25))
                c = (char)('0' + '1' - c);
            s[i] = c;
        }
    }
    s[n] = '\0';
    top = small ? 2 : 3;
    if (top > n)
        top = n;
    k = rnd_chance(80) ? (int)rnd(1, top) : (int)rnd(1, n);
    put_string(s);
    put_int(k);
}
