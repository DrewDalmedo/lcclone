/* A string of m Ls and m Rs. Small cases have m from 1 to 8; full-size
 * ones m = 500, the longest string. The seed picks a shape:
 *  0. the 2m letters in a random order;
 *  1. a row of balanced blocks of random sizes, each block's letters in a
 *     random order (so a block may split further);
 *  2. m pairs, each LR or RL: the most pieces;
 *  3. m of one letter, then m of the other: one piece, and the difference
 *     reaches m on one side of 0;
 *  4. one piece that opens with a letter and closes with the other, the
 *     difference never back to 0 in between, though the letters change
 *     many times. */

/* Writes k of each letter into s in a random order. */
static void mixed(char *s, int k)
{
    int *bits = malloc((size_t)(2 * k) * sizeof *bits), i;

    for (i = 0; i < 2 * k; i++)
        bits[i] = i < k;
    rnd_shuffle(bits, 2 * k);
    for (i = 0; i < 2 * k; i++)
        s[i] = bits[i] ? 'L' : 'R';
    free(bits);
}

void gen(int small)
{
    int m = small ? (int)rnd(1, 8) : 500;
    int shape = (int)rnd(0, 4);
    int pos = 0, k, i, depth, opens, pct;
    char a = rnd_chance(50) ? 'L' : 'R';
    char b = a == 'L' ? 'R' : 'L';
    char *s = malloc((size_t)(2 * m) + 1);

    if (shape == 0) {
        mixed(s, m);
    } else if (shape == 1) {
        while (pos < 2 * m) {
            k = m - pos / 2;
            if (k > (small ? 3 : 20))
                k = small ? 3 : 20;
            k = (int)rnd(1, k);
            mixed(s + pos, k);
            pos += 2 * k;
        }
    } else if (shape == 2) {
        for (i = 0; i < m; i++) {
            if (rnd_chance(50)) {
                s[2 * i] = 'L';
                s[2 * i + 1] = 'R';
            } else {
                s[2 * i] = 'R';
                s[2 * i + 1] = 'L';
            }
        }
    } else if (shape == 3) {
        for (i = 0; i < m; i++) {
            s[i] = a;
            s[m + i] = b;
        }
    } else {
        /* a, then m - 1 pairs in which the count of b never passes the
         * count of a, then b */
        pct = (int)rnd(30, 70);
        s[0] = a;
        s[2 * m - 1] = b;
        opens = m - 1;
        depth = 0;
        for (i = 1; i < 2 * m - 1; i++) {
            if (opens > 0 && (depth == 0 || rnd_chance(pct))) {
                s[i] = a;
                opens--;
                depth++;
            } else {
                s[i] = b;
                depth--;
            }
        }
    }
    s[2 * m] = '\0';
    put_string(s);
    free(s);
}
