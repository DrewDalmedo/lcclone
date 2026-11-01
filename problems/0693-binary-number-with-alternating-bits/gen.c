/* One positive int of L binary digits, the top one a 1: L is 1 to 12 for
 * small cases and 28 to 31 for full-size ones, up to the largest
 * alternating value, 1431655765, and past it. Below the top digit the
 * seed picks a shape: the alternating pattern (twice as likely as the
 * others, since it is the only one that answers true), that pattern with
 * one digit flipped (a single equal pair, anywhere from the top to the
 * bottom), random digits, all zeros (a power of two) or all ones. */
void gen(int small)
{
    int len = small ? (int)rnd(1, 12) : (int)rnd(28, 31);
    int shape = (int)rnd(0, 5), bit, pos, i;
    long long n = 0;

    for (i = len - 1; i >= 0; i--) {
        if (i == len - 1)
            bit = 1;
        else if (shape <= 2)
            bit = (len - 1 - i) % 2 == 0;
        else if (shape == 3)
            bit = rnd_chance(50);
        else
            bit = shape == 5;
        n = n * 2 + bit;
    }
    if (shape == 2 && len > 1) {
        pos = (int)rnd(0, len - 2);
        n ^= 1LL << pos;
    }
    put_int(n);
}
