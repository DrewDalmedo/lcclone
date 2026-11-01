/* Small cases: 1 to 4 pairs (2 to 8 values) from -4 to 4, so that equal
 * values, zeros and negatives come up often, or now and then values within
 * 4 of a bound, -10^4 or 10^4. Full-size ones have 2 * 10^4 values, in a
 * shape the seed picks: random over the whole range; sorted (slow for a
 * quicksort that pivots on an end); sorted the other way; all equal; up to
 * 50 values with many copies each, about half of them an odd number of
 * times (for counting sorts, which must carry the pairing from one value
 * to the next); or a random even length. */
static void sort_values(int *a, int n, int down)
{
    int *count = calloc(20001, sizeof *count);
    int i, v, k = 0;

    for (i = 0; i < n; i++)
        count[a[i] + 10000]++;
    for (v = 0; v <= 20000; v++)
        for (i = 0; i < count[v]; i++) {
            a[down ? n - 1 - k : k] = v - 10000;
            k++;
        }
    free(count);
}

void gen(int small)
{
    int pool[50];
    int n, i, k, v, shape;
    int *nums;

    if (small) {
        n = 2 * (int)rnd(1, 4);
        nums = malloc((size_t)n * sizeof *nums);
        if (rnd_chance(15)) {
            for (i = 0; i < n; i++) {
                v = (int)rnd(9996, 10000);
                nums[i] = rnd_chance(50) ? -v : v;
            }
        } else {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(-4, 4);
        }
        put_ints(nums, n);
        free(nums);
        return;
    }
    shape = (int)(gen_seed() % 6);
    n = shape == 5 ? 2 * (int)rnd(1, 10000) : 20000;
    nums = malloc((size_t)n * sizeof *nums);
    switch (shape) {
    case 3: /* every value the same */
        v = (int)rnd(-10000, 10000);
        for (i = 0; i < n; i++)
            nums[i] = v;
        break;
    case 4: /* a few values, many copies of each */
        k = (int)rnd(2, 50);
        for (i = 0; i < k; i++)
            pool[i] = (int)rnd(-10000, 10000);
        for (i = 0; i < n; i++)
            nums[i] = pool[rnd(0, k - 1)];
        break;
    default: /* random values, sorted for shapes 1 and 2 */
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-10000, 10000);
        if (shape == 1 || shape == 2)
            sort_values(nums, n, shape == 2);
        break;
    }
    put_ints(nums, n);
    free(nums);
}
