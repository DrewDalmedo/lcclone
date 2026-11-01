/* Two random numbers. Small cases often take them from the ends of the
 * range or near zero; full-size ones are anywhere from -100 to 100. */
static int pick(int small)
{
    static const int edge[] = {-100, -99, -1, 0, 1, 99, 100};

    if (small && rnd_chance(40))
        return edge[rnd(0, 6)];
    if (small && rnd_chance(30))
        return (int)rnd(-5, 5);
    return (int)rnd(-100, 100);
}

void gen(int small)
{
    int num1 = pick(small);
    int num2 = pick(small);

    put_int(num1);
    put_int(num2);
}
