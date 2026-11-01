/* Small cases have 1 to 8 characters, either random or built from blocks
 * of 1 to 3 characters, so that ties come up often. Full-size cases have
 * 100 characters, in a shape the seed picks: random; blocks of 1 to 6;
 * one long block of either character at the start, the end or in the
 * middle; a longest block of each character, their lengths equal or one
 * apart; all one character; alternating; or one character filling most of
 * s in short blocks while the other has the longest block. */

/* Fills s[0..n) with blocks of 1 to top characters, alternating between
 * the two characters. */
static void blocks(char *s, int n, int top)
{
    char c = rnd_char("01");
    int i = 0, len, k;

    while (i < n) {
        len = (int)rnd(1, top);
        for (k = 0; k < len && i < n; k++)
            s[i++] = c;
        c = c == '0' ? '1' : '0';
    }
}

/* Writes len copies of c at s[at..at+len), with the other character just
 * outside them wherever s goes on, so that the block is exactly len long. */
static void put_block(char *s, int n, int at, int len, char c)
{
    char other = c == '0' ? '1' : '0';
    int k;

    for (k = 0; k < len; k++)
        s[at + k] = c;
    if (at > 0)
        s[at - 1] = other;
    if (at + len < n)
        s[at + len] = other;
}

void gen(int small)
{
    char s[101], c, other;
    int n = small ? (int)rnd(1, 8) : 100, i, k, shape, len, len2, at, where;

    s[n] = '\0';
    if (small) {
        if (rnd_chance(50)) {
            for (i = 0; i < n; i++)
                s[i] = rnd_char("01");
        } else {
            blocks(s, n, 3);
        }
        put_string(s);
        return;
    }
    shape = (int)(gen_seed() % 7);
    c = rnd_char("01");
    other = c == '0' ? '1' : '0';
    if (shape == 0) {
        for (i = 0; i < n; i++)
            s[i] = rnd_char("01");
    } else if (shape == 1) {
        blocks(s, n, 6);
    } else if (shape == 2) {
        /* One block of 10 to 60 among blocks of at most 4. */
        blocks(s, n, 4);
        len = (int)rnd(10, 60);
        where = (int)rnd(0, 2);
        if (where == 0)
            at = 0;
        else if (where == 1)
            at = n - len;
        else
            at = (int)rnd(1, n - len - 1);
        put_block(s, n, at, len, c);
    } else if (shape == 3) {
        /* A block of c in the first half and one of the other character
         * in the second, among blocks of at most 3 (4 with a flank). */
        blocks(s, n, 3);
        len = (int)rnd(5, 30);
        len2 = len + (int)rnd(-1, 1);
        at = (int)rnd(1, 49 - len);
        put_block(s, n, at, len, c);
        at = (int)rnd(51, 99 - len2);
        put_block(s, n, at, len2, other);
    } else if (shape == 4) {
        for (i = 0; i < n; i++)
            s[i] = c;
    } else if (shape == 5) {
        for (i = 0; i < n; i++)
            s[i] = i % 2 == 0 ? c : other;
    } else {
        /* c in blocks of 1 to 4 split by single copies of the other
         * character, then 5 to 8 of the other in a row somewhere. */
        i = 0;
        while (i < n) {
            len = (int)rnd(1, 4);
            for (k = 0; k < len && i < n; k++)
                s[i++] = c;
            if (i < n)
                s[i++] = other;
        }
        len = (int)rnd(5, 8);
        at = (int)rnd(0, n - len);
        for (k = 0; k < len; k++)
            s[at + k] = other;
    }
    put_string(s);
}
