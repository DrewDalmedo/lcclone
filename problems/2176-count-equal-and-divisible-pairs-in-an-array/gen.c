/* Small cases: up to 8 values from a run of at most 3 numbers, so that
 * equal values are common; the run sometimes starts at 1 or ends at 100.
 * k is 1, small (2 to 12, where products of indices up to 7 land on its
 * multiples), composite (4, 6, 8, 9, 10, 12: two indices can multiply to a
 * multiple of k though neither is one) or anything up to 100.
 * Full-size cases have 100 values; the seed picks all equal, from a few
 * numbers, random in 1..100, or all different (a shuffle of 1..100), and k
 * is random or one of 1, 2, 60, 97 and 100. */
void gen(int small)
{
    static const int composite[] = {4, 6, 8, 9, 10, 12};
    static const int big[] = {1, 2, 60, 97, 100};
    int nums[100];
    int n, span, base, shape, v, k, i;

    if (small) {
        n = (int)rnd(1, 8);
        span = (int)rnd(1, 3);
        shape = (int)rnd(0, 3);
        if (shape == 0)
            base = 1;
        else if (shape == 1)
            base = 100 - span + 1;
        else
            base = (int)rnd(1, 100 - span + 1);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, span - 1);
            nums[i] = base + v;
        }
        shape = (int)rnd(0, 3);
        if (shape == 0)
            k = 1;
        else if (shape == 1)
            k = (int)rnd(2, 12);
        else if (shape == 2)
            k = composite[rnd(0, 5)];
        else
            k = (int)rnd(1, 100);
        put_ints(nums, n);
        put_int(k);
        return;
    }
    n = 100;
    shape = (int)(gen_seed() % 4);
    if (shape == 0) {
        v = (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 1) {
        span = (int)rnd(2, 5);
        base = (int)rnd(1, 100 - span + 1);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, span - 1);
            nums[i] = base + v;
        }
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    }
    k = rnd_chance(50) ? big[rnd(0, 4)] : (int)rnd(1, 100);
    put_ints(nums, n);
    put_int(k);
}
