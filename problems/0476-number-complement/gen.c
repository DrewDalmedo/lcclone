/* One positive int. Small cases have 1 to 31 binary digits; full-size
 * ones always have 31 (bit 30 set), where a mask of 31 ones must be made
 * without overflowing an int. Below the highest 1, the bits follow a
 * pattern: random, mostly ones, mostly zeros, all zeros (a power of two),
 * all ones (2^L - 1, whose complement is 0) or alternating. Small cases
 * pick it at random, full-size ones by seed. */
void gen(int small)
{
    int shape, width, on, i;
    long long num;

    if (small) {
        width = (int)rnd(1, 31);
        shape = (int)rnd(0, 5);
    } else {
        shape = (int)(gen_seed() % 6);
        width = 31;
    }
    num = 1LL << (width - 1);
    for (i = width - 2; i >= 0; i--) {
        on = shape == 1 ? rnd_chance(50)
           : shape == 2 ? rnd_chance(85)
           : shape == 3 ? rnd_chance(15)
           : shape == 4 ? 0
           : shape == 5 ? 1
           : (width - 1 - i) % 2 == 0;
        if (on)
            num |= 1LL << i;
    }
    put_int(num);
}
