/* One n from 1 to 1000. Small cases are mostly small n, with one of the
 * ends of the range (1, 2, 999, 1000) or the spot where a 16-bit total
 * overflows (180 to 182) a third of the time. Full-size cases come from
 * the top of the range, where the totals are largest. */
void gen(int small)
{
    static const int ends[7] = {1, 2, 180, 181, 182, 999, 1000};
    int n;

    if (!small)
        n = (int)rnd(900, 1000);
    else if (rnd_chance(33))
        n = ends[rnd(0, 6)];
    else
        n = (int)rnd(1, 40);
    put_int(n);
}
