/* A range of ball numbers. Small cases are short ranges (up to 60 balls),
 * often placed just below or across a power of ten or at the top of the
 * range (around 99999 and 100000), where digit sums jump. Full-size cases
 * take a shape the seed picks:
 *   0: the whole range 1..10^5 (the largest answer, 6000);
 *   1: a random wide range;
 *   2: a range ending at or near 10^5, through the largest digit sums;
 *   3: a single ball or a few, anywhere. */
void gen(int small)
{
    static const int anchor[] = {10, 100, 1000, 10000, 100000,
                                 90000, 99000, 99900, 99990};
    int shape = small ? -1 : (int)(gen_seed() % 4);
    int lo, hi, t, k;

    if (shape == 0) {
        lo = 1;
        hi = 100000;
    } else if (shape == 1) {
        lo = (int)rnd(1, 100000);
        hi = (int)rnd(1, 100000);
        if (lo > hi) {
            t = lo;
            lo = hi;
            hi = t;
        }
    } else if (shape == 2) {
        lo = (int)rnd(1, 99000);
        hi = (int)rnd(99990, 100000);
    } else if (shape == 3) {
        lo = (int)rnd(1, 100000);
        hi = lo + (int)rnd(0, 3);
        if (hi > 100000)
            hi = 100000;
    } else {
        if (rnd_chance(60)) {
            k = (int)rnd(0, 8);
            lo = anchor[k] - (int)rnd(0, 40);
        } else {
            lo = (int)rnd(1, 100000);
        }
        if (lo < 1)
            lo = 1;
        hi = lo + (int)rnd(0, 59);
        if (hi > 100000)
            hi = 100000;
    }
    put_int(lo);
    put_int(hi);
}
