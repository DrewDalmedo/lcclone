/* Random weights and a threshold k that is, by the seed, 1, the largest
 * weight allowed, one of the weights, one more than a weight, or random.
 * If no weight then reaches k, one weight is raised to k or more (with
 * k = 10^9 that often leaves a single heavy apple). Small cases: up to 8
 * weights from 1 to 10, so weights equal to k come up often. Full size:
 * mostly 50 weights, spread from 1 to 10^9 or bunched within a few grams
 * of k. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(1, 8) : (rnd_chance(80) ? 50 : (int)rnd(1, 50));
    int top = small ? 10 : 1000000000;
    int how = (int)rnd(0, 4), bunched = !small && rnd_chance(50);
    int k, i, at, best;
    long long v;

    if (how == 0)
        k = 1;
    else if (how == 1)
        k = top;
    else
        k = (int)rnd(1, top);
    for (i = 0; i < n; i++) {
        if (bunched) {
            v = k + rnd(-3, 3);
            nums[i] = v < 1 ? 1 : v > top ? top : (int)v;
        } else {
            nums[i] = (int)rnd(1, top);
        }
    }
    at = (int)rnd(0, n - 1);
    if (how == 2)
        k = nums[at];
    else if (how == 3)
        k = nums[at] < top ? nums[at] + 1 : top;
    best = nums[0];
    for (i = 1; i < n; i++)
        if (nums[i] > best)
            best = nums[i];
    if (best < k) {
        at = (int)rnd(0, n - 1);
        nums[at] = (int)rnd(k, top);
    }
    put_ints(nums, n);
    put_int(k);
}
