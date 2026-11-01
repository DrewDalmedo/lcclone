/* The seed picks a shape: random scores; a group of k close scores
 * scattered among scores spread far apart (so that the best k are not
 * next to each other in nums); or scores from a handful of values, with
 * many ties. Small cases have 1 to 8 scores, mostly from 0 to 20 so that
 * ties come up, and any k. Full-size cases have 1000 scores up to 10^5,
 * with k from the whole range, often near n / 2 (the most groups to try)
 * and sometimes n. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int top = small ? (rnd_chance(70) ? 20 : 100000) : 100000;
    int shape = (int)rnd(0, 2);
    int *nums = malloc((size_t)n * sizeof *nums);
    int *pos = malloc((size_t)n * sizeof *pos);
    int i, k, base, width, v, r;

    if (small) {
        k = (int)rnd(1, n);
    } else {
        r = (int)rnd(0, 3);
        if (r == 0)
            k = (int)rnd(1, n);
        else if (r == 3)
            k = n;
        else
            k = (int)rnd(n / 2 - 50, n / 2 + 50);
    }
    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(0, top);
    } else if (shape == 1) {
        width = (int)rnd(0, top / 10);
        base = (int)rnd(0, top - width);
        for (i = 0; i < n; i++)
            nums[i] = rnd_chance(50) ? 0 : top;
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, top);
            if (v < base || v > base + width)
                nums[i] = v;
        }
        for (i = 0; i < n; i++)
            pos[i] = i;
        rnd_shuffle(pos, n);
        for (i = 0; i < k; i++) {
            v = (int)rnd(base, base + width);
            nums[pos[i]] = v;
        }
    } else {
        int vals[4];

        for (i = 0; i < 4; i++)
            vals[i] = (int)rnd(0, top);
        for (i = 0; i < n; i++) {
            r = (int)rnd(0, 3);
            nums[i] = vals[r];
        }
    }
    put_ints(nums, n);
    put_int(k);
    free(pos);
    free(nums);
}
