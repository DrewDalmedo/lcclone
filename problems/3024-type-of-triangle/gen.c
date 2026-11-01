/* Three side lengths, shuffled so that the longest can be at any index.
 * Small cases mostly use lengths up to 8, so that equal sides and flat
 * triples come up often; full-size ones use lengths up to 100 and often
 * include a 100. The shape is picked at random: any three lengths; two of
 * them equal; all three equal; a flat triple (a + b == c); or a nearly
 * flat one (a + b == c - 1 or c + 1). */
void gen(int small)
{
    int top = (small && rnd_chance(75)) ? 8 : 100;
    int shape = (int)rnd(0, 4);
    int s[3], x, y, c;

    s[0] = (int)rnd(1, top);
    s[1] = (int)rnd(1, top);
    s[2] = (int)rnd(1, top);
    if (!small && rnd_chance(50))
        s[0] = 100;
    if (shape == 1) {
        s[1] = s[0];
    } else if (shape == 2) {
        s[1] = s[0];
        s[2] = s[0];
    } else if (shape >= 3) {
        x = (int)rnd(1, top - 1);
        y = (int)rnd(1, top - x);
        c = x + y;
        if (shape == 4)
            c += rnd_chance(50) ? 1 : -1;
        if (c > top)
            c = x + y - 1;
        s[0] = x;
        s[1] = y;
        s[2] = c;
    }
    rnd_shuffle(s, 3);
    put_ints(s, 3);
}
