/* Small cases are up to 8 values with a top the seed picks (3, 10 or
 * 100), sometimes sorted, so that equal neighbours and long rising slices
 * come up often. Full-size cases are 100 values, in a shape the seed
 * picks: random; rising slices of random lengths placed one after
 * another; exactly 1, 2, ..., 100; never rising (sorted from high to low);
 * or small values with one rising slice of large values at the very end. */

/* Fills a[0..len) with len different values from lo..hi, in increasing
 * order. */
static void rising(int *a, int len, int lo, int hi)
{
    int pool[100], m = hi - lo + 1, i, j, t;

    for (i = 0; i < m; i++)
        pool[i] = lo + i;
    for (i = 0; i < len; i++) {
        j = (int)rnd(i, m - 1);
        t = pool[i];
        pool[i] = pool[j];
        pool[j] = t;
    }
    for (i = 1; i < len; i++)
        for (j = i; j > 0 && pool[j - 1] > pool[j]; j--) {
            t = pool[j];
            pool[j] = pool[j - 1];
            pool[j - 1] = t;
        }
    for (i = 0; i < len; i++)
        a[i] = pool[i];
}

static void sort(int *a, int n, int up)
{
    int i, j, t;

    for (i = 1; i < n; i++)
        for (j = i; j > 0 && (up ? a[j - 1] > a[j] : a[j - 1] < a[j]); j--) {
            t = a[j];
            a[j] = a[j - 1];
            a[j - 1] = t;
        }
}

void gen(int small)
{
    static const int tops[] = {3, 10, 100};
    int nums[100], n = small ? (int)rnd(1, 8) : 100, top, i, len, shape;

    if (small) {
        top = tops[rnd(0, 2)];
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
        if (rnd_chance(30))
            sort(nums, n, 1);
    } else {
        shape = (int)(gen_seed() % 5);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(1, 100);
        } else if (shape == 1) {
            for (i = 0; i < n; i += len) {
                len = (int)rnd(1, n - i < 30 ? n - i : 30);
                rising(nums + i, len, 1, 100);
            }
        } else if (shape == 2) {
            for (i = 0; i < n; i++)
                nums[i] = i + 1;
        } else if (shape == 3) {
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(1, 100);
            sort(nums, n, 0);
        } else {
            len = (int)rnd(10, 30);
            for (i = 0; i < n - len; i++)
                nums[i] = (int)rnd(1, 30);
            rising(nums + n - len, len, 60, 100);
        }
    }
    put_ints(nums, n);
}
