/* Five different cards. The seed picks the shape: a flush (one suit, five
 * different ranks from a random run); five cards drawn from a small pool of
 * up to 6 ranks in a row and up to 4 suits, so that pairs, threes, four of
 * a kind, full houses and four cards of one suit come up often; or five
 * cards from the whole deck. A hand always has five cards, so small and
 * full-size cases are made alike. */
void gen(int small)
{
    static const char letters[] = "abcd";
    int pool[52], ranks[5], n = 0, shape, span, lo, nsuits, first, r, s, i;
    char suits[5];

    (void)small;
    shape = (int)rnd(0, 3);
    if (shape == 0) {
        span = (int)rnd(5, 13);
        lo = (int)rnd(1, 14 - span);
        s = (int)rnd(0, 3);
        for (r = lo; r < lo + span; r++)
            pool[n++] = r;
        rnd_shuffle(pool, n);
        for (i = 0; i < 5; i++) {
            ranks[i] = pool[i];
            suits[i] = letters[s];
        }
    } else {
        if (shape == 3) {
            span = 13;
            nsuits = 4;
        } else {
            do {
                span = (int)rnd(1, 6);
                nsuits = (int)rnd(1, 4);
            } while (span * nsuits < 5);
        }
        lo = (int)rnd(1, 14 - span);
        first = (int)rnd(0, 4 - nsuits);
        for (r = lo; r < lo + span; r++)
            for (s = first; s < first + nsuits; s++)
                pool[n++] = r * 4 + s;
        rnd_shuffle(pool, n);
        for (i = 0; i < 5; i++) {
            ranks[i] = pool[i] / 4;
            suits[i] = letters[pool[i] % 4];
        }
    }
    put_ints(ranks, 5);
    put_chars(suits, 5);
}
