/* A three-digit n. Small cases mostly stay at or below 333, where the row
 * has nine digits: often n has three different digits and no 0 (where
 * the fascinating numbers are), else any n up to 333, or n near 333 or
 * the ends of the range. Full-size cases are any n from 100 to 999. */
void gen(int small)
{
    int shape = (int)rnd(0, 4), a, b, c, n;

    if (!small) {
        n = (int)rnd(100, 999);
    } else if (shape <= 1) {
        a = (int)rnd(1, 3);
        do
            b = (int)rnd(1, 9);
        while (b == a);
        do
            c = (int)rnd(1, 9);
        while (c == a || c == b);
        n = 100 * a + 10 * b + c;
    } else if (shape == 2) {
        n = (int)rnd(100, 333);
    } else if (shape == 3) {
        n = (int)rnd(325, 342);
    } else {
        n = rnd_chance(50) ? (int)rnd(100, 109) : (int)rnd(990, 999);
    }
    put_int(n);
}
