/* A stack of numbers from 1 to n holding each of 1..k at least once. The
 * seed picks the shape: random numbers; a needed number only at the
 * bottom (the answer is n), with k often above 31; the top k cards a
 * permutation of 1..k (the answer is k); or a permutation of 1..n with
 * k = n. Small cases use n <= 8, so numbers repeat often. */

/* A number from 1 to n other than skip (0 skips nothing). */
static int other(int n, int skip)
{
    int x;

    if (!skip)
        return (int)rnd(1, n);
    if (n == 1)
        return 1;
    x = (int)rnd(1, n - 1);
    return x >= skip ? x + 1 : x;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 50;
    int shape = (int)rnd(0, 3);
    int nums[50], idx[50], k, bottom = 0, i, v;

    for (i = 0; i < n; i++)
        idx[i] = i;
    if (shape == 3) {
        k = n;
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
        rnd_shuffle(nums, n);
    } else if (shape == 2) {
        k = (int)rnd(1, n);
        for (i = 0; i < n - k; i++)
            nums[i] = (int)rnd(1, n);
        for (i = 0; i < k; i++)
            idx[i] = i + 1;
        rnd_shuffle(idx, k);
        for (i = 0; i < k; i++)
            nums[n - k + i] = idx[i];
    } else {
        if (shape == 1)
            k = small ? (int)rnd(1, n) : (int)rnd(31, n);
        else
            k = (int)rnd(1, n);
        if (shape == 1 && n > 1) {
            /* bottom: a needed number found nowhere else */
            bottom = (int)rnd(1, k);
            nums[0] = bottom;
            for (i = 1; i < n; i++)
                nums[i] = other(n, bottom);
            rnd_shuffle(idx + 1, n - 1);
            for (v = 1, i = 1; v <= k; v++)
                if (v != bottom)
                    nums[idx[i++]] = v;
        } else {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(1, n);
            rnd_shuffle(idx, n);
            for (v = 1; v <= k; v++)
                nums[idx[v - 1]] = v;
        }
    }
    put_ints(nums, n);
    put_int(k);
}
