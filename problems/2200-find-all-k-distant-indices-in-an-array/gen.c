/* Small cases have 1 to 8 entries of a few small values, key at a rate
 * the seed picks (at least once), and k of 1 or 2 more often than not,
 * else any k from 1 to n. Full-size cases have 1000 entries, in a shape the
 * seed picks: key at a rate from 1% to 20% with k up to 10, one or two
 * keys within 10 of one end with k from n - 50 to n, or 2 to 20 keys
 * anywhere with k up to 60. */

/* A value from 1 to 1000 other than key. */
static int other(int key)
{
    int v = (int)rnd(1, 999);

    return v >= key ? v + 1 : v;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int *nums = malloc((size_t)n * sizeof *nums);
    int key, k, top, shape, pct, c, i, p;

    if (small) {
        top = (int)rnd(2, 4);
        key = (int)rnd(1, top);
        pct = (int)rnd(10, 40);
        for (i = 0; i < n; i++) {
            if (rnd_chance(pct)) {
                nums[i] = key;
                continue;
            }
            nums[i] = (int)rnd(1, top - 1);
            if (nums[i] >= key)
                nums[i]++;
        }
        p = (int)rnd(0, n - 1);
        nums[p] = key;
        if (rnd_chance(60))
            k = (int)rnd(1, n < 2 ? n : 2);
        else
            k = (int)rnd(1, n);
    } else {
        key = (int)rnd(1, 1000);
        shape = (int)rnd(0, 2);
        for (i = 0; i < n; i++)
            nums[i] = other(key);
        if (shape == 0) {
            pct = (int)rnd(1, 20);
            for (i = 0; i < n; i++)
                if (rnd_chance(pct))
                    nums[i] = key;
            p = (int)rnd(0, n - 1);
            nums[p] = key;
            k = (int)rnd(1, 10);
        } else if (shape == 1) {
            c = (int)rnd(1, 2);
            for (i = 0; i < c; i++) {
                p = (int)rnd(0, 9);
                nums[rnd_chance(50) ? p : n - 1 - p] = key;
            }
            k = rnd_chance(30) ? n : (int)rnd(n - 50, n);
        } else {
            c = (int)rnd(2, 20);
            for (i = 0; i < c; i++) {
                p = (int)rnd(0, n - 1);
                nums[p] = key;
            }
            k = (int)rnd(1, 60);
        }
    }
    put_ints(nums, n);
    put_int(key);
    put_int(k);
    free(nums);
}
