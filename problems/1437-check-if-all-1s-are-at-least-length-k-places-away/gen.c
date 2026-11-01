/* Recordings of four shapes, picked by the seed: random bits; pulses spaced
 * k quiet ticks apart or a little more, sometimes with one pair moved in to
 * k - 1 (the last pair, in a full-size case); all ones or all zeros; and
 * the edges k = n, n - 1 and n - 2 with at most two pulses. Small cases
 * have a few ticks and a small k; full-size ones 10^5 ticks, and k from 0
 * up to half of that. */
static int pick_k(int small, int n)
{
    int t;

    if (small)
        return (int)rnd(0, n < 4 ? n : 4);
    t = (int)rnd(0, 4);
    if (t == 0)
        return 0;
    if (t == 1)
        return 1;
    if (t == 2)
        return (int)rnd(2, 10);
    if (t == 3)
        return (int)rnd(100, 1000);
    return (int)rnd(10000, n / 2);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 12) : 100000;
    int shape = (int)rnd(0, 3);
    int *nums = calloc((size_t)n, sizeof *nums);
    int *at;
    int k, i, pct, pos, count, t, extra;

    if (shape == 0) {
        pct = (int)rnd(5, 70);
        k = small ? (int)rnd(0, n < 4 ? n : 4) : (int)rnd(0, 2);
        for (i = 0; i < n; i++)
            nums[i] = rnd_chance(pct);
    } else if (shape == 1) {
        k = pick_k(small, n);
        at = malloc((size_t)n * sizeof *at);
        count = 0;
        pos = (int)rnd(0, small ? 2 : 5);
        while (pos < n) {
            at[count++] = pos;
            nums[pos] = 1;
            extra = 0;
            if (rnd_chance(50))
                extra = (int)rnd(1, small ? 2 : 3);
            pos += k + 1 + extra;
        }
        /* Move one pulse in, leaving k - 1 quiet ticks before it. */
        if (k >= 1 && count >= 2 && rnd_chance(50)) {
            t = small ? (int)rnd(0, count - 2) : count - 2;
            nums[at[t + 1]] = 0;
            nums[at[t] + k] = 1;
        }
        free(at);
    } else if (shape == 2) {
        t = rnd_chance(75);
        for (i = 0; i < n; i++)
            nums[i] = t;
        k = small ? (int)rnd(0, n < 2 ? n : 2) : 0;
    } else {
        k = n - (int)rnd(0, 2);
        if (k < 0)
            k = 0;
        t = (int)rnd(0, 2);
        if (t == 0) {
            nums[0] = 1;
            nums[n - 1] = 1;
        } else if (t == 1) {
            pos = (int)rnd(0, n - 1);
            nums[pos] = 1;
        } else {
            pos = (int)rnd(0, n - 1);
            nums[pos] = 1;
            pos = (int)rnd(0, n - 1);
            nums[pos] = 1;
        }
    }
    put_ints(nums, n);
    put_int(k);
    free(nums);
}
