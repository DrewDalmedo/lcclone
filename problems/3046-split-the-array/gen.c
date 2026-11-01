/* Piles of even length over k values, base + 1 to base + k, where base
 * is 0 or 100 - k (so 1 and 100 both come up). The seed picks the shape:
 * half the time no value on more than two cards (every value offered
 * twice, shuffled, the first n kept: true); else the same with one card's
 * value then copied onto two other cards (a value on three or more cards:
 * false), or every card random. Small cases: up to 8 cards and at most
 * 6 values, so equal cards come up often. Full size: mostly 100 cards,
 * over up to 100 values. */
void gen(int small)
{
    int slots[200], nums[100];
    int n, k, base, shape, i, at, b, c, lo, hi, v;

    if (small)
        n = 2 * (int)rnd(1, 4);
    else
        n = rnd_chance(70) ? 100 : 2 * (int)rnd(1, 50);
    if (small)
        k = (int)rnd(n / 2, 6);
    else
        k = rnd_chance(50) ? 100 : (int)rnd(n / 2, 100);
    base = rnd_chance(30) ? 100 - k : 0;
    shape = (int)rnd(0, 3);
    if (shape == 3) {
        for (i = 0; i < n; i++)
            nums[i] = base + (int)rnd(1, k);
    } else {
        for (i = 0; i < k; i++) {
            slots[2 * i] = base + i + 1;
            slots[2 * i + 1] = base + i + 1;
        }
        rnd_shuffle(slots, 2 * k);
        for (i = 0; i < n; i++)
            nums[i] = slots[i];
        if (shape == 2 && n >= 4) {
            at = (int)rnd(0, n - 1);
            v = nums[at];
            b = (int)rnd(0, n - 2);
            if (b >= at)
                b++;
            lo = at < b ? at : b;
            hi = at < b ? b : at;
            c = (int)rnd(0, n - 3);
            if (c >= lo)
                c++;
            if (c >= hi)
                c++;
            nums[b] = v;
            nums[c] = v;
        }
    }
    put_ints(nums, n);
}
