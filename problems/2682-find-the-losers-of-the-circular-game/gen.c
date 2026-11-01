/* Small cases: n up to 12, or a power of two up to 16 (where an odd k
 * reaches every pad, so the answer is empty). Full-size ones: the seed
 * picks n = 50, n = 32 with an odd k (the longest games, 32 jumps for
 * k = 1), k = n (the largest answers), or any n from 33 to 50. */
void gen(int small)
{
    static const int pow2[] = {1, 2, 4, 8, 16, 32};
    int shape = (int)rnd(0, 3), n, k;

    if (small) {
        if (shape == 0)
            n = pow2[rnd(0, 4)];
        else
            n = (int)rnd(1, 12);
        k = (int)rnd(1, n);
    } else if (shape == 0) {
        n = 50;
        k = (int)rnd(1, n);
    } else if (shape == 1) {
        n = pow2[5];
        k = 2 * (int)rnd(0, 15) + 1;
    } else if (shape == 2) {
        n = (int)rnd(1, 50);
        k = n;
    } else {
        n = (int)rnd(33, 50);
        k = (int)rnd(1, n);
    }
    put_int(n);
    put_int(k);
}
