/* One n from 1 to 150. Small cases pick one of the ends (1, 2, 149, 150)
 * a third of the time; full-size ones come from the upper half, where the
 * results are largest. */
void gen(int small)
{
    static const int ends[4] = {1, 2, 149, 150};
    int n;

    if (!small)
        n = (int)rnd(75, 150);
    else if (rnd_chance(33))
        n = ends[rnd(0, 3)];
    else
        n = (int)rnd(1, 150);
    put_int(n);
}
