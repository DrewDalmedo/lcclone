/* Three positions. Small cases put everyone within a short stretch of the
 * line, somewhere from 1 to 100, so shared positions and ties come up
 * often. Full-size cases use the whole range, with the ends 1 and 100
 * likely. Either way the seed sometimes places persons 1 and 2 at mirror
 * positions around z. */
static int place(int small, int base, int span)
{
    if (!small && rnd_chance(15))
        return rnd_chance(50) ? 1 : 100;
    return base + (int)rnd(1, span);
}

void gen(int small)
{
    int span = small ? (int)rnd(1, 8) : 100;
    int base = small ? (int)rnd(0, 100 - span) : 0;
    int x, y, z, d, reach;

    z = place(small, base, span);
    if (rnd_chance(25)) {
        reach = z - 1 < 100 - z ? z - 1 : 100 - z;
        if (small && reach > span)
            reach = span;
        d = (int)rnd(0, reach);
        if (rnd_chance(50)) {
            x = z - d;
            y = z + d;
        } else {
            x = z + d;
            y = z - d;
        }
    } else {
        x = place(small, base, span);
        y = place(small, base, span);
    }
    put_int(x);
    put_int(y);
    put_int(z);
}
