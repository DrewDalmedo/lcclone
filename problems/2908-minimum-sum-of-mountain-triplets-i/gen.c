/* Numbers from 1 to top. The seed picks the shape: random; sorted up or
 * down (no mountain triplet); sorted up except for a smaller last element
 * (every mountain triplet ends there); or all from 45 to 50, so that the
 * costs reach their largest values. Small cases have at most 8 elements,
 * mostly from a narrow range, so that equal values are common. */
static void sort_up(int *a, int n)
{
    int i, j, v;

    for (i = 1; i < n; i++) {
        v = a[i];
        for (j = i; j > 0 && a[j - 1] > v; j--)
            a[j] = a[j - 1];
        a[j] = v;
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : 50;
    int shape = (int)rnd(0, 6);
    int top = small && rnd_chance(80) ? (int)rnd(2, 8) : 50;
    int lo = shape == 6 ? 45 : 1;
    int nums[50], i, t;

    if (lo > top)
        top = 50;
    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(lo, top);
    if (shape == 3 || shape == 4 || shape == 5)
        sort_up(nums, n);
    if (shape == 4)
        for (i = 0; i < n / 2; i++) {
            t = nums[i];
            nums[i] = nums[n - 1 - i];
            nums[n - 1 - i] = t;
        }
    if (shape == 5 && nums[n - 2] > 1)
        nums[n - 1] = (int)rnd(1, nums[n - 2] - 1);
    put_ints(nums, n);
}
