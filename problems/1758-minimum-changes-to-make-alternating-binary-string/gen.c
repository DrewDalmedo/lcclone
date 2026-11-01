/* Small cases have up to 12 digits; full-size ones have 10^4. The seed
 * picks the shape: random digits; an alternating string (starting with
 * either digit) with some digits changed, few or many; runs of equal
 * digits of random lengths (up to 4 when small, up to 50 otherwise); or
 * one digit repeated. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 12) : 10000;
    int shape = (int)rnd(0, 3), i, start, percent, len, d = (int)rnd(0, 1);
    char *s = malloc((size_t)n + 1);

    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("01");
    } else if (shape == 1) {
        start = (int)rnd(0, 1);
        percent = (int)rnd(1, 50);
        for (i = 0; i < n; i++) {
            s[i] = (char)('0' + (start + i) % 2);
            if (rnd_chance(percent))
                s[i] = s[i] == '0' ? '1' : '0';
        }
    } else if (shape == 2) {
        for (i = 0; i < n; d ^= 1) {
            len = (int)rnd(1, small ? 4 : 50);
            while (len-- > 0 && i < n)
                s[i++] = (char)('0' + d);
        }
    } else {
        for (i = 0; i < n; i++)
            s[i] = (char)('0' + d);
    }
    s[n] = '\0';
    put_string(s);
    free(s);
}
