/* Prints the codes numbered codes[0..n) (code c is the letters
 * base + c / k and base + c % k) as a string array. */
static void put_codes(const int *codes, int n, int k, int base)
{
    char buf[50][3];
    char *words[50];
    int i;

    for (i = 0; i < n; i++) {
        buf[i][0] = (char)('a' + base + codes[i] / k);
        buf[i][1] = (char)('a' + base + codes[i] % k);
        buf[i][2] = '\0';
        words[i] = buf[i];
    }
    put_strings(words, n);
}

/* Small cases: up to 8 different codes over 2 to 4 neighboring letters, so
 * that mirrors, doubled letters and codes sharing one letter come up all
 * the time. Full-size cases have 50 codes; the seed picks them from 8
 * letters (many mirrors), from all 26 (few), or as 25 pairs of mirrors in
 * a random order (every entry matched). */
void gen(int small)
{
    int codes[676];
    int n, k, base, shape, a, b, i, m;

    if (small) {
        k = (int)rnd(2, 4);
        base = (int)rnd(0, 26 - k);
        for (i = 0; i < k * k; i++)
            codes[i] = i;
        rnd_shuffle(codes, k * k);
        m = k * k < 8 ? k * k : 8;
        n = (int)rnd(1, m);
        put_codes(codes, n, k, base);
        return;
    }
    n = 50;
    shape = (int)(gen_seed() % 3);
    if (shape == 2) {
        /* 25 different letter pairs a < b, each with its mirror. */
        for (i = 0; i < 676; i++)
            codes[i] = 0;
        m = 0;
        while (m < 25) {
            a = (int)rnd(0, 25);
            b = (int)rnd(0, 25);
            if (a >= b || codes[a * 26 + b])
                continue;
            codes[a * 26 + b] = 1;
            m++;
        }
        m = 0;
        for (i = 0; i < 676; i++)
            if (codes[i] && i / 26 < i % 26)
                codes[m++] = i;
        for (i = 0; i < 25; i++)
            codes[25 + i] = codes[i] % 26 * 26 + codes[i] / 26;
        rnd_shuffle(codes, 50);
        put_codes(codes, n, 26, 0);
        return;
    }
    k = shape == 0 ? 8 : 26;
    base = (int)rnd(0, 26 - k);
    for (i = 0; i < k * k; i++)
        codes[i] = i;
    rnd_shuffle(codes, k * k);
    put_codes(codes, n, k, base);
}
