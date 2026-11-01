/* One random array and a k. The seed picks how the values are drawn: from
 * 1 to 6 (1 to 30 at full size), or from a pool of one to four values, so
 * that repeats are common and nums often has fewer than k different
 * values; just below 10^9, so that neighbours differ by 1; the largest
 * value (at most 8 in small cases) repeated several times above smaller
 * ones; or freely from 1 to 10^9. k is 1, n, or anything between. Small
 * cases have 1 to 8 entries; full-size ones have 100. */
void gen(int small)
{
    int nums[100], pool[4];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 4);
    int kind = (int)rnd(0, 3);
    int k, p, r, top, idx, i;

    k = kind == 0 ? 1 : kind == 1 ? n : (int)rnd(1, n);
    p = (int)rnd(1, 4);
    for (i = 0; i < p; i++)
        pool[i] = (int)rnd(1, 1000000000);
    top = (int)rnd(2, small ? 8 : 1000000000);
    r = (int)rnd(1, n / 2 + 1);
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            nums[i] = (int)rnd(1, small ? 6 : 30);
        } else if (shape == 1) {
            idx = (int)rnd(0, p - 1);
            nums[i] = pool[idx];
        } else if (shape == 2) {
            nums[i] = 1000000000 - (int)rnd(0, small ? 4 : 60);
        } else if (shape == 3) {
            nums[i] = i < r ? top : (int)rnd(1, top - 1);
        } else {
            nums[i] = (int)rnd(1, 1000000000);
        }
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
    put_int(k);
}
