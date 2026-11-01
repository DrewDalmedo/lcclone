/* Two random words, then a target: half the time the sum itself (with a's
 * in front when there is room), else the sum off by a power of ten, the
 * sum with its digits reversed (same digit total), or a random word. Small
 * cases have words of 1 to 3 letters, rich in a and j, and targets of up
 * to 4. Full-size cases reach 8 letters, in a shape the seed picks: two
 * words of 8 random letters; two of 8 letters, mostly j (the sum needs 9
 * digits, more than any target holds); random lengths; or a word of j's
 * plus a short one, so that a carry runs across every place. */

/* Writes v as a word of len letters, with a's in front; v < 10^len. */
static void encode(long long v, int len, char *w)
{
    int i;

    w[len] = '\0';
    for (i = len - 1; i >= 0; i--) {
        w[i] = (char)('a' + v % 10);
        v /= 10;
    }
}

static int digit_count(long long v)
{
    int d = 1;

    while (v >= 10) {
        v /= 10;
        d++;
    }
    return d;
}

/* Fills w with len letters: a with a chance of apct in 100, else j with
 * a chance of jpct, else any of a to j. Returns the number it encodes. */
static long long make_word(char *w, int len, int apct, int jpct)
{
    long long v = 0;
    int i;

    for (i = 0; i < len; i++) {
        if (rnd_chance(apct))
            w[i] = 'a';
        else if (rnd_chance(jpct))
            w[i] = 'j';
        else
            w[i] = rnd_char("abcdefghij");
        v = v * 10 + (w[i] - 'a');
    }
    w[len] = '\0';
    return v;
}

void gen(int small)
{
    char a[9], b[9], t[9];
    int ttop = small ? 4 : 8, la, lb, lt, mode, shape, d;
    long long va, vb, sum, want, p, r;

    shape = small ? -1 : (int)(gen_seed() % 4);
    if (shape == -1) {
        la = (int)rnd(1, 3);
        lb = (int)rnd(1, 3);
        va = make_word(a, la, 30, 30);
        vb = make_word(b, lb, 30, 30);
    } else if (shape == 0) {
        va = make_word(a, 8, 10, 10);
        vb = make_word(b, 8, 10, 10);
    } else if (shape == 1) {
        va = make_word(a, 8, 0, 70);
        vb = make_word(b, 8, 0, 70);
    } else if (shape == 2) {
        la = (int)rnd(1, 8);
        lb = (int)rnd(1, 8);
        va = make_word(a, la, 30, 30);
        vb = make_word(b, lb, 30, 30);
    } else {
        la = (int)rnd(1, 8);
        lb = (int)rnd(1, 3);
        va = make_word(a, la, 0, 95);
        vb = make_word(b, lb, 40, 0);
    }
    sum = va + vb;
    mode = (int)rnd(0, 9);
    want = -1;
    if (mode < 5) {
        want = sum;
    } else if (mode < 8) {
        d = (int)rnd(0, digit_count(sum) - 1);
        for (p = 1; d > 0; d--)
            p *= 10;
        want = sum + p;
        if (sum >= p && rnd_chance(50))
            want = sum - p;
    } else if (mode == 8) {
        for (r = 0, p = sum; p > 0; p /= 10)
            r = r * 10 + p % 10;
        want = r;
    }
    if (want >= 0 && digit_count(want) <= ttop) {
        lt = (int)rnd(digit_count(want), ttop);
        encode(want, lt, t);
    } else {
        lt = (int)rnd(1, ttop);
        make_word(t, lt, 30, 30);
    }
    put_string(a);
    put_string(b);
    put_string(t);
}
