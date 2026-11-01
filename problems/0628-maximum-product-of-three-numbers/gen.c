/* Small cases: 3 to 8 values from -5 to 5, so that zeros, negatives and
 * repeats come up often, or now and then values within 4 of a bound,
 * -1000 or 1000. Full-size ones have 10^4 values, in a shape the seed
 * picks: random over the whole range; sorted; sorted the other way; all
 * negative; all equal; or mostly small values with a few negatives of
 * large size, so that two of them times the largest value win. */
static void sort_values(int *a, int n, int down)
{
    int count[2001] = {0};
    int i, v, k = 0;

    for (i = 0; i < n; i++)
        count[a[i] + 1000]++;
    for (v = 0; v <= 2000; v++)
        for (i = 0; i < count[v]; i++) {
            a[down ? n - 1 - k : k] = v - 1000;
            k++;
        }
}

void gen(int small)
{
    int n, i, v, shape;
    int *nums;

    if (small) {
        n = (int)rnd(3, 8);
        nums = malloc((size_t)n * sizeof *nums);
        if (rnd_chance(15)) {
            for (i = 0; i < n; i++) {
                v = (int)rnd(996, 1000);
                nums[i] = rnd_chance(50) ? -v : v;
            }
        } else {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(-5, 5);
        }
        put_ints(nums, n);
        free(nums);
        return;
    }
    n = 10000;
    nums = malloc((size_t)n * sizeof *nums);
    shape = (int)(gen_seed() % 6);
    switch (shape) {
    case 3: /* every value negative */
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-1000, -1);
        break;
    case 4: /* every value the same */
        v = (int)rnd(-1000, 1000);
        for (i = 0; i < n; i++)
            nums[i] = v;
        break;
    case 5: /* small values, and a few large negatives */
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-50, 100);
        for (i = 0; i < 5; i++) {
            v = (int)rnd(0, n - 1);
            nums[v] = (int)rnd(-1000, -900);
        }
        break;
    default: /* random values, sorted for shapes 1 and 2 */
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-1000, 1000);
        if (shape == 1 || shape == 2)
            sort_values(nums, n, shape == 2);
        break;
    }
    put_ints(nums, n);
    free(nums);
}
