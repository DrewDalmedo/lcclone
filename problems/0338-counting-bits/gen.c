/* One n. Small cases are 0 to 20, a power of two from 2^0 to 2^8 or one
 * of its neighbours (where one-pass methods that track the highest power
 * of two slip), or now and then up to 300. Full-size ones, as the seed
 * picks: 10^5, the largest n; 2^16 or a neighbour of it (2^16 - 1 is the
 * first number with sixteen 1s); or any n from 5 * 10^4 up. */
void gen(int small)
{
    long long n, k;
    int shape = (int)rnd(0, 9);

    if (small) {
        if (shape < 5) {
            n = rnd(0, 20);
        } else if (shape < 9) {
            k = rnd(0, 8);
            n = (1LL << k) + rnd(-1, 1);
        } else {
            n = rnd(21, 300);
        }
    } else {
        switch (gen_seed() % 3) {
        case 0:
            n = 100000;
            break;
        case 1:
            n = 65536 + rnd(-1, 1);
            break;
        default:
            n = rnd(50000, 100000);
        }
    }
    put_int(n);
}
