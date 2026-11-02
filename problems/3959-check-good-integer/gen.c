/* The seed picks a shape: the digits of a core whose Q - D is exactly 50
 * or 48 (or 42 with Q = 50), padded with 0s and 1s and shuffled; digits
 * 0 to 7 only (no single digit is enough); digits 0 and 1 only (or 10^9
 * itself); or any value. Small cases have up to 6 digits, full-size ones
 * up to 9, or 10 for 10^9. */
static const char *const cores[] = {
    "65", "732", "6432", "5443", "72222", "55322", "44442",
    "73", "643", "7222", "54422", "6333", "44433", "71"
};

void gen(int small)
{
    int maxlen = small ? 6 : 9;
    int shape = (int)rnd(0, 3), len, clen, i, j, t;
    int d[10];
    long long v = 0;
    const char *core;

    if (shape == 3) {
        put_int(rnd(1, small ? 99999 : 1000000000));
        return;
    }
    if (shape == 2 && !small && rnd_chance(25)) {
        put_int(1000000000);
        return;
    }
    if (shape == 0) {
        core = cores[rnd(0, 13)];
        clen = (int)strlen(core);
        len = (int)rnd(clen, maxlen);
        for (i = 0; i < len; i++)
            d[i] = i < clen ? core[i] - '0' : (int)rnd(0, 1);
        rnd_shuffle(d, len);
    } else {
        len = (int)rnd(1, maxlen);
        for (i = 0; i < len; i++)
            d[i] = (int)rnd(0, shape == 1 ? 7 : 1);
    }
    for (j = 0; j < len && d[j] == 0; j++)
        ;
    if (j == len) {
        d[0] = 1;
    } else {
        t = d[0];
        d[0] = d[j];
        d[j] = t;
    }
    for (i = 0; i < len; i++)
        v = v * 10 + d[i];
    put_int(v);
}
