/* Puts t distinct values from 1 to m in a[0..t), in increasing order. */
static void distinct_sorted(int *a, int t, int m)
{
    int v, k = 0;

    for (v = 1; v <= m && k < t; v++)
        if (rnd(1, m - v + 1) <= t - k)
            a[k++] = v;
}

/* The seed picks a shape: values drawn at random; a random walk with
 * steps from -2 to 2, which makes rises, falls and flat stretches; one
 * strictly rising or falling array; or random values followed by a long
 * rising or falling tail, so the best segment ends the array. Small
 * cases have 1 to 8 elements from a narrow range, so that equal
 * neighbours are common; full-size ones have 50 elements from 1 to 50. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(1, 8) : 50, shape = (int)rnd(0, 3);
    int top = small ? (int)rnd(1, 6) : 50, m, p, i, step, tmp;

    if (shape == 0) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
    } else if (shape == 1) {
        nums[0] = (int)rnd(1, top);
        for (i = 1; i < n; i++) {
            step = (int)rnd(-2, 2);
            nums[i] = nums[i - 1] + step;
            if (nums[i] < 1)
                nums[i] = 1;
            if (nums[i] > top)
                nums[i] = top;
        }
    } else {
        p = shape == 2 ? 0 : (int)rnd(0, n / 2);
        for (i = 0; i < p; i++)
            nums[i] = (int)rnd(1, top);
        m = small ? (int)rnd(n - p, n - p + 6) : 50;
        distinct_sorted(nums + p, n - p, m);
        if (rnd_chance(50))
            for (i = 0; i < (n - p) / 2; i++) {
                tmp = nums[p + i];
                nums[p + i] = nums[n - 1 - i];
                nums[n - 1 - i] = tmp;
            }
    }
    put_ints(nums, n);
}
