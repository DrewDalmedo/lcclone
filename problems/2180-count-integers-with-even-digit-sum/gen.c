/* Small cases: num up to 120 (crossing the tens and the first hundred),
 * often right at a boundary such as 9, 10, 99 or 100, and sometimes
 * anything up to 1000. Full-size cases: num is 1000 itself for one seed in
 * four, else between 900 and 1000. */
void gen(int small)
{
    static const int edge[] = {1, 9, 10, 11, 19, 20, 99, 100, 101, 999, 1000};
    int shape;

    if (small) {
        shape = (int)rnd(0, 3);
        if (shape == 0)
            put_int(edge[rnd(0, 10)]);
        else if (shape == 3)
            put_int(rnd(1, 1000));
        else
            put_int(rnd(1, 120));
        return;
    }
    put_int(gen_seed() % 4 == 0 ? 1000 : rnd(900, 1000));
}
