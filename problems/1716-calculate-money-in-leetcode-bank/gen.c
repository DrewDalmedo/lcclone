/* A number of days. Small cases cover the first few weeks; full-size ones
 * are mostly in the last weeks before 1000, and sometimes exactly 1000, or
 * a whole number of weeks. */
void gen(int small)
{
    int shape = (int)rnd(0, 3), n;

    if (small)
        n = (int)rnd(1, 30);
    else if (shape == 0)
        n = 1000;
    else if (shape == 1)
        n = 7 * (int)rnd(1, 142);
    else
        n = (int)rnd(900, 1000);
    put_int(n);
}
