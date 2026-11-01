/* Small cases have 1 to 8 cells, mostly white, mostly black or even, so
 * that answers of 0 and k come up often. Full-size cases have 100 cells,
 * in a shape the seed picks: random, with a random share of black; runs
 * of 1 to 8 cells of each colour; sparse black cells with the best group
 * at the very end; all one colour; or a long black run among
 * whites on the left, beaten near the right end by two shorter runs a
 * few whites apart. */

/* Fills s[0..n) at random, each cell black with a chance of black%. */
static void scatter(char *s, int n, int black)
{
    int i;

    for (i = 0; i < n; i++)
        s[i] = rnd_chance(black) ? 'B' : 'W';
}

static void run(char *s, int at, int len, char c)
{
    int i;

    for (i = 0; i < len; i++)
        s[at + i] = c;
}

void gen(int small)
{
    static const int shares[] = {15, 50, 85};
    char s[101], c;
    int n = small ? (int)rnd(1, 8) : 100, k, shape, i, len, white, a, b, gap;

    s[n] = '\0';
    if (small) {
        scatter(s, n, shares[rnd(0, 2)]);
        k = (int)rnd(1, n);
        put_string(s);
        put_int(k);
        return;
    }
    shape = (int)(gen_seed() % 5);
    k = rnd_chance(30) ? 50 : (int)rnd(1, n);
    if (shape == 0) {
        scatter(s, n, (int)rnd(5, 95));
    } else if (shape == 1) {
        c = rnd_char("BW");
        for (i = 0; i < n; i += len) {
            len = (int)rnd(1, 8);
            if (len > n - i)
                len = n - i;
            run(s, i, len, c);
            c = c == 'B' ? 'W' : 'B';
        }
    } else if (shape == 2) {
        /* One cell in five black, then the last k cells black but for at
         * most three whites, so that the best group is at the very end. */
        k = (int)rnd(10, 90);
        scatter(s, n, 20);
        run(s, n - k, k, 'B');
        white = (int)rnd(0, 3);
        for (i = 0; i < white; i++) {
            a = (int)rnd(n - k + 1, n - 1);
            s[a] = 'W';
        }
    } else if (shape == 3) {
        c = rnd_char("BW");
        run(s, 0, n, c);
    } else {
        /* A run of len blacks inside the first 50 cells, and two runs of
         * a and b blacks, gap whites apart, ending at the last cell, with
         * a + b > len. The group of k = a + gap + b + 1 cells that ends
         * at the last cell starts on a white. */
        run(s, 0, n, 'W');
        len = (int)rnd(4, 15);
        i = (int)rnd(1, 50 - len);
        run(s, i, len, 'B');
        a = (int)rnd(len / 2 + 1, len);
        b = (int)rnd(len / 2 + 1, len);
        gap = (int)rnd(1, 3);
        k = a + gap + b + 1;
        run(s, n - b, b, 'B');
        run(s, n - b - gap - a, a, 'B');
    }
    put_string(s);
    put_int(k);
}
