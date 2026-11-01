/* One n. Small cases are mostly tiny, but sometimes anywhere in the
 * range, so that the cross-check with brute.c reaches the points where
 * unreduced factorials overflow. Full-size ones favour the top of the
 * range: the seed picks n = 100, n from 90 to 100, or any n. */
void gen(int small)
{
    int n, shape;

    if (small) {
        if (rnd_chance(80))
            n = (int)rnd(1, 20);
        else
            n = (int)rnd(1, 100);
    } else {
        shape = (int)rnd(0, 2);
        if (shape == 0)
            n = 100;
        else if (shape == 1)
            n = (int)rnd(90, 100);
        else
            n = (int)rnd(1, 100);
    }
    put_int(n);
}
