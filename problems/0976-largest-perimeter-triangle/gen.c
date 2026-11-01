/* Stick lengths, shuffled at the end.
 *
 * Small cases: 3 to 8 lengths in a shape the seed picks:
 *   0. random, mostly from 1..12 (now and then up to 10^6);
 *   1. a chain that grows at least as fast as Fibonacci (each length the
 *      sum of the two before it, or a little more), so no three make a
 *      triangle, with one length sometimes replaced by a random one;
 *   2. only 1, 2, 3 and 4 (repeats, flat triples, equal sides);
 *   3. extremes: 1, 500000, 999999 and 10^6.
 *
 * Full-size cases: 10^4 lengths in a shape the seed picks:
 *   0. random up to 10^6;
 *   1. a Fibonacci-like chain from about 35 up to nearly 10^6, the rest
 *      from 1..30: the longest sticks fail one after another, and the
 *      answer comes from near the bottom of the chain;
 *   2. every length near 10^6, or all exactly 10^6 (the largest answer);
 *   3. all 1s but for one or two sticks of 10^6. */
static int chain(int *a, int max, int first, int len)
{
    int k = 0, next, extra;

    a[k++] = first;
    extra = (int)rnd(0, 5);
    if (len > 1 && first + extra <= max)
        a[k++] = first + extra;
    while (k < len) {
        extra = rnd_chance(60) ? 0 : (int)rnd(1, 3);
        next = a[k - 1] + a[k - 2] + extra;
        if (next > max)
            break;
        a[k++] = next;
    }
    return k;
}

void gen(int small)
{
    static const int tiny[] = {1, 2, 3, 4};
    static const int edge[] = {1, 500000, 999999, 1000000};
    int n = small ? (int)rnd(3, 8) : 10000, i, k, top, pos;
    int shape = small ? (int)rnd(0, 3) : (int)(gen_seed() % 4);
    int *nums = malloc((size_t)n * sizeof *nums);

    if (small && shape == 1) {
        top = (int)rnd(1, 3);
        k = chain(nums, 1000000, top, n);
        for (i = k; i < n; i++)
            nums[i] = (int)rnd(1, 3);
        if (rnd_chance(50)) {
            pos = (int)rnd(0, n - 1);
            nums[pos] = (int)rnd(1, 20);
        }
    } else if (small) {
        top = rnd_chance(15) ? 1000000 : 12;
        for (i = 0; i < n; i++) {
            if (shape == 0)
                nums[i] = (int)rnd(1, top);
            else if (shape == 2)
                nums[i] = tiny[rnd(0, 3)];
            else
                nums[i] = edge[rnd(0, 3)];
        }
    } else if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 1000000);
    } else if (shape == 1) {
        top = (int)rnd(31, 40);
        k = chain(nums, 1000000, top, n);
        for (i = k; i < n; i++)
            nums[i] = (int)rnd(1, 30);
    } else if (shape == 2) {
        top = rnd_chance(50) ? 1000000 : 999000;
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(top, 1000000);
    } else {
        k = (int)rnd(1, 2);
        for (i = 0; i < n; i++)
            nums[i] = i < k ? 1000000 : 1;
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
    free(nums);
}
