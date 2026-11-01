/* An array and a starting value, built around the chain original,
 * 2 * original, 4 * original, ...: the first `len` values of the chain go
 * into the array, the next one is kept out of it, the rest is filled with
 * random values, and the whole is shuffled so the chain is out of order.
 * Small cases use up to 8 values of at most 16, or a start between 126
 * and 1000 whose chain soon passes 1000. Full-size ones have 1000 values,
 * and the seed picks the shape: the longest chain (1 up to 512), a chain
 * that runs past 1000 (often ending at 2000), a chain that stops early,
 * or original missing. */
void gen(int small)
{
    int nums[1000], n, top = 1000, original, len, stop, shape, v, i;

    shape = (int)rnd(0, 3);
    if (small) {
        n = (int)rnd(1, 8);
        if (shape == 0) {
            original = (int)rnd(126, 1000);
        } else {
            top = 16;
            original = (int)rnd(1, 8);
        }
        len = (int)rnd(0, 3);
    } else {
        n = 1000;
        if (shape == 0) {
            original = 1;
            len = 10;
        } else if (shape == 1) {
            v = (int)rnd(0, 3);
            original = rnd_chance(50) ? 1000 >> v : (int)rnd(1, 1000);
            len = 10;
        } else if (shape == 2) {
            original = (int)rnd(1, 300);
            len = (int)rnd(1, 9);
        } else {
            original = (int)rnd(1, 1000);
            len = 0;
        }
    }
    for (i = 0, v = original; i < len && i < n && v <= top; i++, v *= 2)
        nums[i] = v;
    stop = v;
    for (; i < n; i++) {
        do
            nums[i] = (int)rnd(1, top);
        while (nums[i] == stop);
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
    put_int(original);
}
