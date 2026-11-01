/* A number and a frame width k from 1 to its digit count. The seed picks
 * one of four shapes: digits drawn from a small set with 0 in it, so that
 * frames with leading zeros and frames of zeros come up often; a block of
 * digits written several times over (such as 373737), so that the same
 * frame occurs at several positions and divides num; a few digits followed
 * by zeros; or 10^9 itself, the only ten-digit num. Small cases have 1 to
 * 10 digits; full-size ones 8 to 10, the most frames. */
void gen(int small)
{
    static const char *sets[] = {"0123456789", "012", "0125", "0369",
                                 "048", "05"};
    int shape = (int)rnd(0, 9), d, block, reps, i, k;
    long long num = 0, b, p;
    const char *digits;
    char c;

    if (shape == 9) {
        num = 1000000000;
        d = 10;
    } else if (shape >= 6) {
        /* A block of 1 to 4 digits, repeated to fill up to 9 digits. */
        block = (int)rnd(1, 4);
        reps = small ? (int)rnd(1, 9 / block) : 9 / block;
        b = rnd(1, 9);
        for (i = 1; i < block; i++) {
            c = rnd_char("0123456789");
            b = b * 10 + (c - '0');
        }
        p = 1;
        for (i = 0; i < block; i++)
            p *= 10;
        for (i = 0; i < reps; i++)
            num = num * p + b;
        d = block * reps;
    } else if (shape >= 4) {
        /* Up to three significant digits, then zeros. */
        d = small ? (int)rnd(1, 9) : 9;
        num = rnd(1, 9);
        for (i = 1; i < d; i++) {
            c = i < 3 ? rnd_char("0123456789") : '0';
            num = num * 10 + (c - '0');
        }
    } else {
        d = small ? (int)rnd(1, 9) : 9;
        digits = sets[rnd(0, 5)];
        num = rnd(1, 9);
        for (i = 1; i < d; i++) {
            c = rnd_char(digits);
            num = num * 10 + (c - '0');
        }
    }
    k = (int)rnd(1, d);
    put_int(num);
    put_int(k);
}
