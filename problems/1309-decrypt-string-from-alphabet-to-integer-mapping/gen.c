/* Encodes a random word. Small cases have a few letters, mostly from the
 * edges of the two kinds of piece (a, b, i, j, l, t, y, z), so that a lone
 * 1 or 2 often stands right before a two-digit letter, and zeros show up.
 * Full-size ones fill s to exactly 1000 characters, by a shape the seed
 * picks: one-digit letters only, two-digit letters only, or a mix. */
static int put_piece(char *s, int len, int v)
{
    if (v < 10) {
        s[len++] = (char)('0' + v);
    } else {
        s[len++] = (char)('0' + v / 10);
        s[len++] = (char)('0' + v % 10);
        s[len++] = '#';
    }
    return len;
}

void gen(int small)
{
    static const int edge[] = {1, 2, 9, 10, 12, 20, 25, 26};
    char s[1001];
    int len = 0, count, shape, i, v;

    if (small) {
        count = (int)rnd(1, 6);
        for (i = 0; i < count; i++) {
            if (rnd_chance(60))
                v = edge[rnd(0, 7)];
            else
                v = (int)rnd(1, 26);
            len = put_piece(s, len, v);
        }
    } else {
        shape = (int)rnd(0, 2);
        while (len < 1000) {
            if (shape == 0 || 1000 - len < 3)
                v = (int)rnd(1, 9);
            else if (shape == 1)
                v = (int)rnd(10, 26);
            else
                v = (int)rnd(1, 26);
            len = put_piece(s, len, v);
        }
    }
    s[len] = '\0';
    put_string(s);
}
