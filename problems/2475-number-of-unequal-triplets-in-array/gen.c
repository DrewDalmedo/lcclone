/* Small cases: 3 to 8 beads in 1 to 6 colours (usually 3 or more), so
 * repeats are common and an answer of 0 comes up now and then. Full-size
 * cases have 100 beads, in a shape the seed picks: 100 different colours
 * (the largest answer, 161700), one to three colours (often an answer
 * of 0), a palette of 3 to 100 colours, or colours drawn freely from 1 to
 * 1000. */
void gen(int small)
{
    int nums[100], palette[100], pool[1000];
    int n, d, shape, i, k, t, v;

    if (small) {
        n = (int)rnd(3, 8);
        d = rnd_chance(25) ? (int)rnd(1, 2) : (int)rnd(3, 6);
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, d);
        put_ints(nums, n);
        return;
    }
    n = 100;
    shape = (int)rnd(0, 3);
    /* d different colours from 1..1000, by a partial shuffle. */
    d = shape == 0 ? 100 : shape == 1 ? (int)rnd(1, 3) : (int)rnd(3, 100);
    for (i = 0; i < 1000; i++)
        pool[i] = i + 1;
    for (i = 0; i < d; i++) {
        k = (int)rnd(i, 999);
        t = pool[i];
        pool[i] = pool[k];
        pool[k] = t;
        palette[i] = pool[i];
    }
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            v = palette[i];
        } else if (shape == 3) {
            v = (int)rnd(1, 1000);
        } else {
            k = (int)rnd(0, d - 1);
            v = palette[k];
        }
        nums[i] = v;
    }
    put_ints(nums, n);
}
