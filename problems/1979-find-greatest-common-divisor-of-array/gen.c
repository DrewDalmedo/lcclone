/* The seed picks a shape: values that are all multiples of one base g;
 * a smallest value lo and a largest value hi that share the base g, with
 * random values between them (which often share less with lo and hi than
 * these two share with each other); plain random values; or every value
 * equal. Small cases have 2 to 8 values, mostly small ones so that ties
 * and repeated extremes come up. Full-size cases have 1000 values up to
 * 1000, with the extremes at random positions. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 1000;
    int shape = (int)rnd(0, 3);
    int top = small ? (rnd_chance(70) ? 30 : 1000) : 1000;
    int *nums = malloc((size_t)n * sizeof *nums);
    int i, g, m, v, a, b, lo, hi, p, q;

    if (shape <= 1) {
        g = (int)rnd(1, small ? 12 : 100);
        m = top / g;
        if (m < 1) {
            g = top;
            m = 1;
        }
        if (shape == 0) {
            for (i = 0; i < n; i++) {
                v = (int)rnd(1, m);
                nums[i] = g * v;
            }
        } else {
            a = (int)rnd(1, m);
            b = (int)rnd(a, m);
            lo = g * a;
            hi = g * b;
            for (i = 0; i < n; i++)
                nums[i] = (int)rnd(lo, hi);
            p = (int)rnd(0, n - 1);
            q = (int)rnd(0, n - 2);
            if (q >= p)
                q++;
            nums[p] = lo;
            nums[q] = hi;
        }
    } else if (shape == 2) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
    } else {
        v = (int)rnd(1, top);
        for (i = 0; i < n; i++)
            nums[i] = v;
    }
    put_ints(nums, n);
    free(nums);
}
