/* One team count. Small cases stay below 20; full-size ones pick by seed
 * the top bound 200, a count from 150 to 200, or a power of two or one
 * more than a power of two (rounds that stay even, or stay odd). */
void gen(int small)
{
    int shape, k, n;

    if (small) {
        n = (int)rnd(1, 19);
    } else {
        shape = (int)rnd(0, 2);
        if (shape == 0) {
            n = 200;
        } else if (shape == 1) {
            n = (int)rnd(150, 200);
        } else {
            k = (int)rnd(0, 7);
            n = (1 << k) + (int)rnd(0, 1);
        }
    }
    put_int(n);
}
