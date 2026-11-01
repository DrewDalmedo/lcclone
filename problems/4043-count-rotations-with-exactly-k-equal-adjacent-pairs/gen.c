/* A string and a target k. Small cases have 2 to 8 letters, full-size ones
 * 100. The seed picks the alphabet (one letter now and then, mostly two or
 * three, sometimes all 26) and the string's shape:
 *   0: random letters;
 *   1: a block of 2 or more letters, not all the same, whose length
 *      divides n, repeated, so that several shifts give the same string
 *      (random letters when n is prime);
 *   2: runs of random lengths, each of a letter other than the run before.
 * k is mostly one of the two counts that some shift has, c or c - 1,
 * where c counts the equal neighbours around the ring (s[n - 1] next to
 * s[0] included), so the answer is seldom 0; otherwise any k from 0 to
 * n - 1. */
static const char *alphabets[] = {"a", "ab", "abc", "abcdefghijklmnopqrstuvwxyz"};

void gen(int small)
{
    char s[101], unit[100];
    const char *alpha;
    int n = small ? (int)rnd(2, 8) : 100;
    int v, shape, divs[100], nd = 0, u, same, len, ch, prev, c = 0, pick, k;
    int i, j;

    v = (int)rnd(0, 19);
    alpha = alphabets[v < 1 ? 0 : v < 9 ? 1 : v < 16 ? 2 : 3];
    shape = (int)rnd(0, 2);
    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char(alpha);
    } else if (shape == 1) {
        for (u = 2; u < n; u++)
            if (n % u == 0)
                divs[nd++] = u;
        if (nd == 0) {
            for (i = 0; i < n; i++)
                s[i] = rnd_char(alpha);
        } else {
            u = divs[rnd(0, nd - 1)];
            same = 1;
            for (i = 0; i < u; i++) {
                unit[i] = rnd_char(alpha);
                if (unit[i] != unit[0])
                    same = 0;
            }
            if (same && alpha[1] != '\0')
                while (unit[u - 1] == unit[0])
                    unit[u - 1] = rnd_char(alpha);
            for (i = 0; i < n; i++)
                s[i] = unit[i % u];
        }
    } else {
        ch = 0;
        for (i = 0; i < n; i += len) {
            len = (int)rnd(1, small ? 3 : 12);
            prev = ch;
            ch = rnd_char(alpha);
            if (alpha[1] != '\0')
                while (ch == prev)
                    ch = rnd_char(alpha);
            for (j = i; j < n && j < i + len; j++)
                s[j] = (char)ch;
        }
    }
    s[n] = '\0';
    for (i = 0; i < n; i++)
        if (s[i] == s[(i + 1) % n])
            c++;
    pick = (int)rnd(0, 9);
    if (pick < 4)
        k = c;
    else if (pick < 7)
        k = c - 1;
    else
        k = (int)rnd(0, n - 1);
    if (k > n - 1)
        k = n - 1;
    if (k < 0)
        k = 0;
    put_string(s);
    put_int(k);
}
