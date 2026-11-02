/* An array and a step k, built around the multiples k, 2k, 3k, ...: the
 * first `len` of them go into the array, the rest is filled with random
 * values (usually never the next multiple, so the chain stops there), and
 * the whole is shuffled, so the multiples come out of order.
 *
 * Small cases hold up to 8 values, and the seed picks the shape: a k from
 * 34 to 100 whose multiples up to 100 all occur, so the answer passes 100;
 * values of at most 10 or 12 and a k of at most 4, so that copies, gaps
 * and values k doesn't divide are common; or any k with a short chain.
 * Full-size ones have 100 values: all of 1 to 100 with k = 1 (the answer
 * 101, after the most steps), every multiple of a small k up to 100, a
 * chain that stops partway, or k itself missing. */
void gen(int small)
{
    int nums[100], n, top = 100, k, len, stop, keep_out = 1, shape, v, i;

    shape = (int)rnd(0, 3);
    if (small) {
        n = (int)rnd(1, 8);
        if (shape == 0) {
            k = (int)rnd(34, 100);
            len = 100 / k;
        } else if (shape == 1) {
            k = (int)rnd(1, 3);
            len = (int)rnd(0, 4);
            top = 10;
            keep_out = 0;
        } else if (shape == 2) {
            k = (int)rnd(1, 4);
            len = (int)rnd(0, 4);
            top = 12;
        } else {
            k = (int)rnd(1, 100);
            len = (int)rnd(0, 2);
        }
    } else {
        n = 100;
        if (shape == 0) {
            k = 1;
            len = 100;
        } else if (shape == 1) {
            k = (int)rnd(1, 10);
            len = 100 / k;
        } else if (shape == 2) {
            k = (int)rnd(1, 20);
            len = (int)rnd(1, 100 / k - 1);
        } else {
            k = (int)rnd(1, 100);
            len = 0;
        }
    }
    for (i = 0, v = k; i < len && i < n && v <= top; i++, v += k)
        nums[i] = v;
    stop = v;
    for (; i < n; i++) {
        do
            nums[i] = (int)rnd(1, top);
        while (keep_out && nums[i] == stop);
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
    put_int(k);
}
