/* nums has 4 to 9 entries in small cases (3 now and then); full-size ones
 * have 100 (or now and then any length from 3 to 99). The seed picks the
 * shape (1 most often, 3 seldom):
 *   0: random values from a small range, so that counts of 3 come by
 *      chance;
 *   1: planted groups, each of a value of its own: three evenly spaced
 *      copies most of the time, else three unevenly spaced ones (in either
 *      order), four or five evenly spaced, two copies, or three evenly
 *      spaced and one more anywhere. The free places get values used once
 *      or one of three "noise" values, which can end up with any count;
 *   3: full size only: n / 3 values, each at i, i + m and i + 2m
 *      (m = n / 3), interleaved, so the answer reaches 33.
 * Values are drawn from 1 to 100 in a random order, so 1 and 100 come up. */
static int nums[100], pool[100], used;

/* Puts value v at at[0..m) if those places are different and free. */
static int place(const int *at, int m, int v)
{
    int i, j;

    for (i = 0; i < m; i++) {
        if (nums[at[i]] != 0)
            return 0;
        for (j = 0; j < i; j++)
            if (at[j] == at[i])
                return 0;
    }
    for (i = 0; i < m; i++)
        nums[at[i]] = v;
    return 1;
}

/* Tries to plant one group of the given kind; returns 1 when it fits. */
static int plant(int n, int kind)
{
    int at[5], m = 3, steps = 2, d, e, a, j;

    if (kind == 6) {
        if (n < 4)
            return 0;
        d = (int)rnd(1, n - 2);
        e = (int)rnd(1, n - 1 - d);
        if (e == d) {
            if (d + e + 1 <= n - 1)
                e++;
            else if (e > 1)
                e--;
            else
                return 0;
        }
        a = (int)rnd(0, n - 1 - d - e);
        at[0] = a;
        at[1] = a + d;
        at[2] = a + d + e;
        return place(at, 3, pool[used]);
    }
    if (kind == 7) {
        m = rnd_chance(70) ? 4 : 5;
        steps = m - 1;
    } else if (kind == 8) {
        m = 2;
        steps = 1;
    }
    if (n - 1 < steps)
        return 0;
    d = (int)rnd(1, (n - 1) / steps);
    a = (int)rnd(0, n - 1 - steps * d);
    for (j = 0; j < m; j++)
        at[j] = a + j * d;
    if (kind == 9) {
        at[3] = (int)rnd(0, n - 1);
        m = 4;
    }
    return place(at, m, pool[used]);
}

void gen(int small)
{
    int n, shape, top, groups, kind, m, i, g, tries, v, k, noise[3];

    if (small)
        n = rnd_chance(15) ? 3 : (int)rnd(4, 9);
    else
        n = rnd_chance(80) ? 100 : (int)rnd(3, 99);
    for (i = 0; i < 100; i++)
        pool[i] = i + 1;
    rnd_shuffle(pool, 100);
    used = 0;
    for (i = 0; i < n; i++)
        nums[i] = 0;
    k = (int)rnd(0, 19);
    shape = k < 5 ? 0 : k < 17 || small ? 1 : 3;
    if (shape == 0) {
        top = small ? (int)rnd(1, 4) : (int)rnd(10, 40);
        for (i = 0; i < n; i++) {
            k = (int)rnd(0, top - 1);
            nums[i] = pool[k];
        }
    } else if (shape == 3) {
        m = n / 3;
        for (i = 0; i < m; i++) {
            v = pool[used++];
            nums[i] = v;
            nums[i + m] = v;
            nums[i + 2 * m] = v;
        }
        for (i = 3 * m; i < n; i++)
            nums[i] = pool[used++];
    } else {
        for (i = 0; i < 3; i++)
            noise[i] = pool[used++];
        groups = small ? (int)rnd(1, 3) : (int)rnd(5, 33);
        for (g = 0; g < groups; g++) {
            kind = (int)rnd(0, 9);
            for (tries = 0; tries < 30; tries++)
                if (plant(n, kind)) {
                    used++;
                    break;
                }
        }
        for (i = 0; i < n; i++)
            if (nums[i] == 0) {
                if (used < 100 && rnd_chance(50)) {
                    nums[i] = pool[used++];
                } else {
                    k = (int)rnd(0, 2);
                    nums[i] = noise[k];
                }
            }
    }
    put_ints(nums, n);
}
