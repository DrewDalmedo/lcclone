/* Small cases: strings of up to 8 letters (so that brute.c can list every
 * subsequence) over a tiny alphabet. Full-size cases: a has 100 letters,
 * and both are over a few letters or all 26. The seed picks how b relates
 * to a: random; equal to a; a's letters, some of them deleted (a proper
 * subsequence, or a prefix or suffix); a with one letter changed; a's
 * letters shuffled; and, half the time, a and b then swap places. */
static void random_string(char *s, int len, const char *abc)
{
    int i;

    for (i = 0; i < len; i++)
        s[i] = rnd_char(abc);
    s[len] = '\0';
}

void gen(int small)
{
    static const char *alphabets[] = {
        "a", "ab", "abc", "xyz", "abcdefghijklmnopqrstuvwxyz"
    };
    const char *abc = alphabets[rnd(small ? 0 : 1, 4)];
    int la = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 6), lb = 0, i, k, cut;
    char a[101], b[101], c, *x, *y;

    random_string(a, la, abc);
    if (shape == 0) {
        lb = small ? (int)rnd(1, 8) : (int)rnd(90, 100);
        random_string(b, lb, abc);
    } else if (shape == 1) {
        lb = la;
        memcpy(b, a, (size_t)la + 1);
    } else if (shape == 2) {
        /* Keep each letter by chance, but never all of them or none. */
        for (i = 0; i < la; i++)
            if (rnd_chance(small ? 50 : 90))
                b[lb++] = a[i];
        if (lb == la)
            lb--;
        if (lb == 0)
            b[lb++] = a[0];
        b[lb] = '\0';
    } else if (shape == 3 || shape == 4) {
        /* A prefix (shape 3) or a suffix (shape 4), shorter than a. */
        lb = la == 1 ? 1 : (int)rnd(1, la - 1);
        cut = shape == 3 ? 0 : la - lb;
        memcpy(b, a + cut, (size_t)lb);
        b[lb] = '\0';
    } else if (shape == 5) {
        lb = la;
        memcpy(b, a, (size_t)la + 1);
        k = (int)rnd(0, la - 1);
        c = rnd_char(abc);
        b[k] = c;
    } else {
        lb = la;
        memcpy(b, a, (size_t)la + 1);
        for (i = la - 1; i > 0; i--) {
            k = (int)rnd(0, i);
            c = b[i];
            b[i] = b[k];
            b[k] = c;
        }
    }
    x = a;
    y = b;
    if (rnd_chance(50)) {
        x = b;
        y = a;
    }
    put_string(x);
    put_string(y);
}
