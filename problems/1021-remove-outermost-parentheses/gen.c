/* A string of m pairs, 2m characters. The seed picks a shape: a random walk
 * that opens and closes about equally often; a row of blocks of random
 * sizes; a walk that mostly opens (deep nesting); a single block nested m
 * deep, the longest result; a walk that mostly closes (many shallow
 * blocks); and, at full size only, m bare () blocks, an empty result. Small
 * cases have 1 to 8 pairs; full-size ones 5 * 10^4 pairs, so the deepest
 * ones pass what a 16-bit counter holds. */

/* Writes 2m characters: '(' with a chance of pct in 100 while one may
 * still be opened, ')' otherwise; the counter never goes below 0. */
static void walk(char *s, int m, int pct)
{
    int opens = m, depth = 0, i;

    for (i = 0; i < 2 * m; i++) {
        if (opens > 0 && (depth == 0 || rnd_chance(pct))) {
            s[i] = '(';
            opens--;
            depth++;
        } else {
            s[i] = ')';
            depth--;
        }
    }
}

void gen(int small)
{
    int m = small ? (int)rnd(1, 8) : 50000;
    int shape = (int)rnd(0, small ? 4 : 5);
    int cap = small ? 4 : (int)rnd(2, 1000);
    int pos = 0, k, i;
    char *s = malloc((size_t)2 * m + 1);

    if (shape == 0) {
        walk(s, m, 50);
    } else if (shape == 1) {
        while (pos < 2 * m) {
            k = m - pos / 2 < cap ? m - pos / 2 : cap;
            k = (int)rnd(1, k);
            s[pos] = '(';
            walk(s + pos + 1, k - 1, 50);
            s[pos + 2 * k - 1] = ')';
            pos += 2 * k;
        }
    } else if (shape == 2) {
        walk(s, m, 85);
    } else if (shape == 3) {
        for (i = 0; i < m; i++) {
            s[i] = '(';
            s[2 * m - 1 - i] = ')';
        }
    } else if (shape == 4) {
        walk(s, m, 25);
    } else {
        for (i = 0; i < m; i++) {
            s[2 * i] = '(';
            s[2 * i + 1] = ')';
        }
    }
    s[2 * m] = '\0';
    put_string(s);
    free(s);
}
