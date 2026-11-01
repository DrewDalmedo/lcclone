/* Small cases have 2 to 8 digits, mostly from 0..2 so that balanced
 * strings come up by chance, sometimes from 0..9. Full-size cases have 99
 * or 100 digits, with the seed picking the shape: any digits, or digits
 * from 7..9, which push the totals to their largest. Half of all cases
 * are then evened out into balanced strings by moving digits up or down
 * one step at a time, and some of those are pushed one step off again. */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(2, 8) : (int)rnd(99, 100);
    int shape = small ? (int)rnd(0, 3) : (int)rnd(0, 1);
    const char *digits = small ? (shape ? "012" : "0123456789")
                               : (shape ? "789" : "0123456789");
    int diff = 0, i, p, step;

    for (i = 0; i < n; i++) {
        s[i] = rnd_char(digits);
        diff += i % 2 == 0 ? s[i] - '0' : '0' - s[i];
    }
    if (rnd_chance(50)) {
        /* Each step moves diff (light total minus dark total) one closer
         * to 0: raise a digit on the lighter side or lower one on the
         * heavier side, when that digit can move. */
        while (diff != 0) {
            p = (int)rnd(0, n - 1);
            step = (p % 2 == 0) == (diff < 0) ? 1 : -1;
            if (s[p] + step < '0' || s[p] + step > '9')
                continue;
            s[p] = (char)(s[p] + step);
            diff += p % 2 == 0 ? step : -step;
        }
        if (rnd_chance(30)) {
            p = (int)rnd(0, n - 1);
            s[p] = (char)(s[p] == '9' ? '8' : s[p] + 1);
        }
    }
    s[n] = '\0';
    put_string(s);
}
