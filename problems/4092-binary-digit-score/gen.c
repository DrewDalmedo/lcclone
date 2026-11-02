/* Small cases: a and b at random, often a power of two, a number with
 * many 1 bits, or equal to each other. Full size: the count of 1 bits of
 * each number is picked first, from 1 to 6, so the largest counts (only 63
 * and 95 have six) come up as often as the others. */
static int bits_of(int x)
{
    int count = 0;

    for (; x > 0; x /= 2)
        count += x % 2;
    return count;
}

static int pick_small(void)
{
    static const int odd[] = { 1, 2, 3, 7, 15, 31, 63, 64, 95, 96, 100 };

    if (rnd_chance(35))
        return odd[rnd(0, 10)];
    return (int)rnd(1, 100);
}

static int pick_with_bits(void)
{
    int want = (int)rnd(1, 6), x;

    do
        x = (int)rnd(1, 100);
    while (bits_of(x) != want);
    return x;
}

void gen(int small)
{
    int a = small ? pick_small() : pick_with_bits();
    int b = small ? pick_small() : pick_with_bits();

    if (rnd_chance(15))
        b = a;
    put_int(a);
    put_int(b);
}
