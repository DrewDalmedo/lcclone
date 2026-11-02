/* A random source square and a target picked by a random shape: the same
 * square, another square on the same row, column, falling diagonal or
 * rising diagonal, a knight's jump away, or any square at all. A shape
 * with no other square on the board from this source (the short diagonal
 * through a corner, say) falls back to any square. The board is always
 * 8 x 8, so small and full-size cases are alike. */
static int fits(int shape, int dr, int dc)
{
    int ar = dr < 0 ? -dr : dr, ac = dc < 0 ? -dc : dc;

    switch (shape) {
    case 0: return ar == 0 && ac == 0;
    case 1: return ar == 0 && ac != 0;
    case 2: return ar != 0 && ac == 0;
    case 3: return ar != 0 && dr == dc;
    case 4: return ar != 0 && dr == -dc;
    case 5: return (ar == 1 && ac == 2) || (ar == 2 && ac == 1);
    default: return 1;
    }
}

void gen(int small)
{
    int src[2], dst[2], cand[64][2], n = 0, shape, r, c, pick;

    (void)small;
    src[0] = (int)rnd(1, 8);
    src[1] = (int)rnd(1, 8);
    shape = (int)rnd(0, 6);
    for (r = 1; r <= 8; r++)
        for (c = 1; c <= 8; c++)
            if (fits(shape, r - src[0], c - src[1])) {
                cand[n][0] = r;
                cand[n][1] = c;
                n++;
            }
    if (n == 0) {
        dst[0] = (int)rnd(1, 8);
        dst[1] = (int)rnd(1, 8);
    } else {
        pick = (int)rnd(0, n - 1);
        dst[0] = cand[pick][0];
        dst[1] = cand[pick][1];
    }
    put_ints(src, 2);
    put_ints(dst, 2);
}
