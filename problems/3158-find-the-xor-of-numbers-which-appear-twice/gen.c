/* Small cases: up to 8 different values from a range of 1..50, each used
 * once or twice, in a random order. The seed picks the range: a narrow one
 * starting at 1, a narrow one ending at 50, a narrow one anywhere, or all
 * of 1..50. Sometimes the doubled values are made to cancel out to 0.
 * Full-size cases have 50 elements and always use both 1 and 50; the seed
 * picks every value doubled, all 50 values once, or a random mix of pairs
 * and singletons, either shuffled or laid out as the first copies, then
 * the singletons, then the second copies (each pair's copies far apart). */
void gen(int small)
{
    int pool[50], nums[50];
    int shape = (int)rnd(0, 3);
    int span, base, k, pairs, n, first, x, i, j, t;

    if (small) {
        span = shape == 3 ? 50 : (int)rnd(1, 8);
        if (shape == 0 || shape == 3)
            base = 1;
        else if (shape == 1)
            base = 50 - span + 1;
        else
            base = (int)rnd(1, 50 - span + 1);
        k = (int)rnd(1, span < 8 ? span : 8);
        pairs = rnd_chance(15) ? 0 : (int)rnd(1, k);
        first = 0;
    } else {
        span = 50;
        base = 1;
        if (shape == 0) {
            pairs = 25;
            k = 25;
        } else if (shape == 1) {
            pairs = 0;
            k = 50;
        } else {
            pairs = (int)rnd(1, 24);
            k = 50 - pairs;
        }
        first = 2;
    }

    /* pool[0..k) becomes k different values of base..base + span - 1;
     * full-size cases keep 1 at pool[0] and put 50 at pool[1]. */
    for (i = 0; i < span; i++)
        pool[i] = base + i;
    if (!small) {
        pool[1] = 50;
        pool[49] = 2;
    }
    for (i = first; i < k; i++) {
        j = (int)rnd(i, span - 1);
        t = pool[i];
        pool[i] = pool[j];
        pool[j] = t;
    }
    /* The first `pairs` values of the pool are the doubled ones. */
    rnd_shuffle(pool, k);
    /* Small cases with 3 or more doubled values sometimes make them cancel
     * out: the last doubled value becomes the XOR of the others, when that
     * is a value of 1..50 that isn't doubled already. */
    if (small && pairs >= 3 && rnd_chance(30)) {
        x = 0;
        for (i = 0; i < pairs - 1; i++)
            x ^= pool[i];
        for (j = 0; j < k && pool[j] != x; j++)
            ;
        if (x >= 1 && x <= 50 && j >= pairs - 1) {
            if (j < k)
                pool[j] = pool[pairs - 1];
            pool[pairs - 1] = x;
        }
    }

    n = k + pairs;
    if (!small && shape == 3) {
        for (i = 0; i < pairs; i++) {
            nums[i] = pool[i];
            nums[k + i] = pool[i];
        }
        for (i = pairs; i < k; i++)
            nums[i] = pool[i];
    } else {
        for (i = 0; i < k; i++)
            nums[i] = pool[i];
        for (i = 0; i < pairs; i++)
            nums[k + i] = pool[i];
        rnd_shuffle(nums, n);
    }
    put_ints(nums, n);
}
