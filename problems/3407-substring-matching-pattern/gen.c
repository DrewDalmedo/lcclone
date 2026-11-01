/* A text s and a pattern p with one '*'. Each part of p is either copied
 * from a random place in s (so that it is found, though maybe in the wrong
 * order or overlapping the other part) or made of random letters. Small
 * cases use up to 8 letters of "ab" or "abc", and parts of 0 to 3 letters,
 * so the '*' often sits at an end. Full-size cases, shaped by the seed:
 * 50 a's with one b near the end, which a naive search scans to the end;
 * random a's and b's; random letters of the whole alphabet; or a short s
 * with a pattern of 50 characters that can't fit in it. */
static void fill(char *dst, int k, const char *s, int n, const char *alpha)
{
    int i, at;

    if (k <= n && rnd_chance(60)) {
        at = (int)rnd(0, n - k);
        memcpy(dst, s + at, (size_t)k);
    } else {
        for (i = 0; i < k; i++)
            dst[i] = rnd_char(alpha);
    }
}

void gen(int small)
{
    char s[51], p[51];
    const char *alpha = "ab";
    int shape = (int)rnd(0, 3), n, llen, rlen, i, at;

    if (small) {
        if (shape == 0)
            alpha = "abc";
        n = (int)rnd(1, 8);
    } else if (shape == 2) {
        alpha = "abcdefghijklmnopqrstuvwxyz";
        n = 50;
    } else if (shape == 3) {
        n = (int)rnd(1, 5);
    } else {
        n = 50;
    }
    for (i = 0; i < n; i++)
        s[i] = shape == 0 && !small ? 'a' : rnd_char(alpha);
    s[n] = '\0';

    if (small) {
        llen = (int)rnd(0, 3);
        rlen = (int)rnd(0, 3);
    } else if (shape == 3) {
        llen = (int)rnd(0, 49);
        rlen = 49 - llen;
    } else {
        llen = (int)rnd(0, 24);
        rlen = (int)rnd(0, 49 - llen);
    }

    if (!small && shape == 0) {
        /* Runs of a's, with the right part ending in the b. */
        at = (int)rnd(40, 49);
        s[at] = 'b';
        if (rlen == 0)
            rlen = 1;
        for (i = 0; i < llen; i++)
            p[i] = 'a';
        for (i = 0; i < rlen - 1; i++)
            p[llen + 1 + i] = 'a';
        p[llen + rlen] = 'b';
    } else {
        fill(p, llen, s, n, alpha);
        fill(p + llen + 1, rlen, s, n, alpha);
    }
    p[llen] = '*';
    p[llen + 1 + rlen] = '\0';
    put_string(s);
    put_string(p);
}
