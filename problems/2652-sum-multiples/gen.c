/* One n. brute.c is quick for any n, so small cases cover the whole range:
 * often below 40, where the first overlaps (15, 21, 35) are, sometimes
 * next to a multiple of 105, and sometimes anywhere up to 1000. Full-size
 * cases are 1000, 999 or above 900. */
void gen(int small)
{
    int shape = (int)rnd(0, 2), m, d;

    if (small) {
        if (shape == 0) {
            put_int(rnd(1, 40));
        } else if (shape == 1) {
            m = 105 * (int)rnd(1, 9);
            d = (int)rnd(-2, 2);
            put_int(m + d);
        } else {
            put_int(rnd(1, 1000));
        }
    } else {
        if (shape == 0)
            put_int(1000);
        else if (shape == 1)
            put_int(999);
        else
            put_int(rnd(900, 1000));
    }
}
