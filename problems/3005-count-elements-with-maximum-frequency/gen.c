/* Small cases: up to 8 entries drawn from a pool of 1 to 4 numbers, each
 * of them 1, 100 or something between, so that ties at the top, a single
 * hit, logs of one song and the value 100 come up often.
 * Full-size cases: 100 entries, in a shape the seed picks:
 *   0: numbers from all of 1..100;
 *   1: every number from 1 to 100 once, in random order (the answer is
 *      100);
 *   2: 2 to 5 numbers tie at the top count, spread out, among numbers
 *      played fewer times;
 *   3: only 1 and 100, in random order. */
void gen(int small)
{
    int shape = small ? -1 : (int)(gen_seed() % 4);
    int n = small ? (int)rnd(1, 8) : 100;
    int nums[100], vals[100], pool[4], p, i, j, k, c, top, filled;

    if (shape == -1) {
        p = (int)rnd(1, 4);
        for (i = 0; i < p; i++) {
            k = (int)rnd(0, 3);
            pool[i] = k == 0 ? 1 : k == 1 ? 100 : (int)rnd(2, 99);
        }
        for (i = 0; i < n; i++)
            nums[i] = pool[rnd(0, p - 1)];
    } else if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 100);
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else if (shape == 2) {
        for (i = 0; i < 100; i++)
            vals[i] = i + 1;
        rnd_shuffle(vals, 100);
        k = (int)rnd(2, 5);
        top = (int)rnd(2, 100 / k);
        filled = 0;
        for (j = 0; j < k; j++)
            for (i = 0; i < top; i++)
                nums[filled++] = vals[j];
        /* the other numbers each get fewer than top entries */
        for (j = k; filled < n; j++) {
            c = (int)rnd(1, top - 1);
            for (i = 0; i < c && filled < n; i++)
                nums[filled++] = vals[j];
        }
        rnd_shuffle(nums, n);
    } else {
        for (i = 0; i < n; i++)
            nums[i] = rnd_chance(50) ? 1 : 100;
    }
    put_ints(nums, n);
}
