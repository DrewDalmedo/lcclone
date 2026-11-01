/* Two numbers from 1 to 50, drawn one after the other. Small cases often
 * take them from the ends of the range or below 10; full-size ones are
 * anywhere in it. */
static int pick(int small)
{
    static const int edge[] = {1, 2, 49, 50};

    if (small && rnd_chance(40))
        return edge[rnd(0, 3)];
    if (small && rnd_chance(40))
        return (int)rnd(1, 9);
    return (int)rnd(1, 50);
}

void gen(int small)
{
    int num = pick(small);
    int t = pick(small);

    put_int(num);
    put_int(t);
}
