/* Each value is drawn by parity first: an odd or an even value up to top.
 * Small cases: 1 to 8 values up to 6 (now and then up to 100), odd with a
 * chance of 0, 30, 50, 70 or 100 percent, so all-even and all-odd arrays
 * come up. Full size: 100 values up to 100, in a shape the seed picks:
 * random parities; mostly one parity; a block of one parity followed by a
 * block of the other; strictly alternating; all one parity; or one value
 * of the other parity at a random index among the rest. */
static int pick(int odd, int top)
{
    int k;

    if (odd) {
        k = (int)rnd(0, (top - 1) / 2);
        return 2 * k + 1;
    }
    k = (int)rnd(1, top / 2);
    return 2 * k;
}

void gen(int small)
{
    static const int chances[5] = {0, 30, 50, 70, 100};
    int n = small ? (int)rnd(1, 8) : 100, i, shape, chance, cut, first, odd;
    int top = small && rnd_chance(80) ? 6 : 100;
    int nums[100];

    shape = small ? 0 : (int)rnd(0, 5);
    chance = small ? chances[rnd(0, 4)] : 50;
    if (shape == 1)
        chance = rnd_chance(50) ? 10 : 90;
    cut = (int)rnd(0, n - 1);
    first = rnd_chance(50);
    for (i = 0; i < n; i++) {
        if (shape <= 1)
            odd = rnd_chance(chance);
        else if (shape == 2)
            odd = first ^ (i > cut);
        else if (shape == 3)
            odd = first ^ (i % 2);
        else if (shape == 4)
            odd = first;
        else
            odd = first ^ (i == cut);
        nums[i] = pick(odd, top);
    }
    put_ints(nums, n);
}
