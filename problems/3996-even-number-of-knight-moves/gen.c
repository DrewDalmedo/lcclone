/* Two squares of the board. Now and then both are the same square, or a
 * coordinate sits on an edge (0 or 7); full-size cases favour squares far
 * apart, near opposite corners. */
static int coord(int edge)
{
    if (edge && rnd_chance(50))
        return rnd_chance(50) ? 0 : 7;
    return (int)rnd(0, 7);
}

void gen(int small)
{
    int start[2], target[2], i;
    int far = !small && rnd_chance(50);

    for (i = 0; i < 2; i++)
        start[i] = far ? (int)rnd(0, 1) : coord(1);
    if (!far && rnd_chance(15)) {
        target[0] = start[0];
        target[1] = start[1];
    } else {
        for (i = 0; i < 2; i++)
            target[i] = far ? (int)rnd(6, 7) : coord(1);
    }
    put_ints(start, 2);
    put_ints(target, 2);
}
