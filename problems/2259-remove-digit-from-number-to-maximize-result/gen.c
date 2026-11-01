/* A number around a chosen digit d: its digits are mostly d and the
 * digits next to it (d - 1 and d + 1), so that runs of d, and copies of d
 * followed by smaller and by larger digits, come up often. One random
 * place is set to d, so it occurs at least once. Small cases have 2 to 7
 * digits. Full-size ones have 100, with the seed picking the shape: the
 * digits around d; any digits from 1 to 9; d = 9, which is never followed
 * by a larger digit; or digits no larger than d, so that no copy is
 * followed by a larger digit except, maybe, one near the end. */
static char pick_digit(int d, int lo, int hi)
{
    int v;

    if (rnd_chance(45))
        return (char)('0' + d);
    v = (int)rnd(lo, hi);
    return (char)('0' + v);
}

void gen(int small)
{
    char number[101];
    int n = small ? (int)rnd(2, 7) : 100;
    int shape = small ? 0 : (int)rnd(0, 3);
    int d = (int)rnd(1, 9), lo, hi, i, at;

    if (shape == 2)
        d = 9;
    lo = d > 1 ? d - 1 : 1;
    hi = d < 9 ? d + 1 : 9;
    if (shape == 1) {
        lo = 1;
        hi = 9;
    } else if (shape == 3) {
        lo = 1;
        hi = d;
    }
    for (i = 0; i < n; i++)
        number[i] = pick_digit(d, lo, hi);
    at = (int)rnd(0, n - 1);
    number[at] = (char)('0' + d);
    if (shape == 3 && d < 9 && rnd_chance(50)) {
        at = (int)rnd(n - 10, n - 2);
        number[at] = (char)('0' + d);
        number[at + 1] = (char)('0' + d + 1);
    }
    number[n] = '\0';
    put_string(number);
    put_char((char)('0' + d));
}
