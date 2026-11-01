/* n + 1 different values from 0..top: the first is repeated n times, the
 * others are written once. Now and then the repeated value is forced to 0
 * or 10^4. The seed picks the layout:
 *   0. shuffled;
 *   1. the copies at the odd positions (a majority vote fails there);
 *   2. the copies at the even positions;
 *   3. the single values first, then all the copies (the slowest shape
 *      for comparing each entry with the later ones);
 *   4. the copies first, then the single values;
 *   5. copies at both ends, the rest shuffled (for n = 2, at positions 0
 *      and 3, as far apart as they can be).
 * Small cases: n from 2 to 4, mostly with values up to n + 4, so that 0
 * and the edges come up often. Full-size cases: n = 5000, values up to
 * 10^4, layouts 0 to 3. */
static int pool[10001];

void gen(int small)
{
    int n = small ? (int)rnd(2, 4) : 5000;
    int top = small && !rnd_chance(20) ? n + (int)rnd(0, 4) : 10000;
    int shape = small ? (int)rnd(0, 5) : (int)(gen_seed() % 4);
    int force = (int)rnd(0, 5), *nums = malloc((size_t)(2 * n) * sizeof *nums);
    int i, j, t, x, *single = pool + 1;

    for (i = 0; i <= top; i++)
        pool[i] = i;
    for (i = 0; i <= n; i++) {
        j = (int)rnd(i, top);
        t = pool[i];
        pool[i] = pool[j];
        pool[j] = t;
    }
    /* force 0 or 1: make 0 or 10^4 (when it is in range) the repeated one. */
    if (force <= 1 && (force == 0 || top == 10000)) {
        t = force == 0 ? 0 : 10000;
        for (i = 1; i <= n && pool[i] != t; i++)
            ;
        if (i <= n)
            pool[i] = pool[0];
        pool[0] = t;
    }
    x = pool[0];
    for (i = 0; i < n; i++)
        nums[i] = single[i];
    rnd_shuffle(nums, n);
    if (shape == 0 || shape == 5) {
        for (i = n; i < 2 * n; i++)
            nums[i] = x;
        rnd_shuffle(nums, 2 * n);
        /* Swap a copy into position 0, then one into the last position. */
        if (shape == 5 && nums[0] != x) {
            for (i = 1; nums[i] != x; i++)
                ;
            nums[i] = nums[0];
            nums[0] = x;
        }
        if (shape == 5 && nums[2 * n - 1] != x) {
            for (i = 1; nums[i] != x; i++)
                ;
            nums[i] = nums[2 * n - 1];
            nums[2 * n - 1] = x;
        }
    } else if (shape == 1 || shape == 2) {
        for (i = n - 1; i >= 0; i--) {
            nums[2 * i + (shape == 1 ? 0 : 1)] = nums[i];
            nums[2 * i + (shape == 1 ? 1 : 0)] = x;
        }
    } else if (shape == 3) {
        for (i = n; i < 2 * n; i++)
            nums[i] = x;
    } else {
        for (i = 0; i < n; i++) {
            nums[n + i] = nums[i];
            nums[i] = x;
        }
    }
    put_ints(nums, 2 * n);
    free(nums);
}
