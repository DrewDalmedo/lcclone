/* One lone value and some pairs, all different values. Small cases have
 * at most 4 pairs, from a range as narrow as they allow, so 0, negative
 * values and a value next to its negation come up often; the elements are
 * shuffled. Full-size cases have 14999 pairs (3 * 10^4 - 1 elements) and
 * always use -3 * 10^4 and 3 * 10^4. The seed picks their order: shuffled;
 * the pairs shuffled and the lone value last; or every pair's copies half
 * the array apart, with the lone value last. */
void gen(int small)
{
    int pairs = small ? (int)rnd(0, 4) : 14999;
    int lo = (pairs + 1) / 2 > 1 ? (pairs + 1) / 2 : 1;
    int top = small ? (int)rnd(lo, 6) : 30000;
    int span = 2 * top + 1, k = pairs + 1, n = 2 * pairs + 1;
    int shape = small ? 0 : (int)rnd(0, 2), first = 0, i, j, t;
    int *pool = malloc((size_t)span * sizeof *pool);
    int *nums = malloc((size_t)n * sizeof *nums);

    /* pool[0..k) becomes k different values of -top..top. */
    for (i = 0; i < span; i++)
        pool[i] = i - top;
    if (!small) {
        /* -top is already at pool[0]; put top at pool[1]. */
        pool[1] = top;
        pool[span - 1] = 1 - top;
        first = 2;
    }
    for (i = first; i < k; i++) {
        j = (int)rnd(i, span - 1);
        t = pool[i];
        pool[i] = pool[j];
        pool[j] = t;
    }
    /* The lone value is any of the k, moved to pool[0]. */
    j = (int)rnd(0, k - 1);
    t = pool[0];
    pool[0] = pool[j];
    pool[j] = t;

    if (shape == 2) {
        for (i = 0; i < pairs; i++) {
            nums[i] = pool[i + 1];
            nums[pairs + i] = pool[i + 1];
        }
    } else {
        for (i = 0; i < pairs; i++) {
            nums[2 * i] = pool[i + 1];
            nums[2 * i + 1] = pool[i + 1];
        }
        rnd_shuffle(nums, n - 1);
    }
    nums[n - 1] = pool[0];
    if (shape == 0)
        rnd_shuffle(nums, n);
    put_ints(nums, n);
    free(pool);
    free(nums);
}
