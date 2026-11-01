/* Entries of 2 to 5 digits. The seed picks a shape: values drawn freely;
 * digits drawn from one small set for the whole case, so ranges often
 * tie; numbers with all digits the same (every range 0); or each entry's
 * digits from its own narrow band (ranges 0 to 3), with now and then a
 * wider entry put first or last. Some cases also get 100000 or 99999, or
 * a copy of an earlier entry. Small cases have 1 to 6 entries, full-size
 * ones 100. */
static int build(const int *digits, int count, int len)
{
    int v = 0, i, d;

    for (i = 0; i < len; i++) {
        d = digits[rnd(0, count - 1)];
        if (i == 0 && d == 0)
            d = digits[0] != 0 ? digits[0] : digits[1];
        v = v * 10 + d;
    }
    return v;
}

static int band(int lo, int hi, int len)
{
    int digits[10], i;

    for (i = lo; i <= hi; i++)
        digits[i - lo] = i;
    if (lo == 0 && hi == 0) {
        digits[0] = 1;
        digits[1] = 0;
        return build(digits, 2, len);
    }
    if (lo == 0) {
        digits[0] = hi;
        digits[hi] = 0;
    }
    return build(digits, hi - lo + 1, len);
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 100;
    int shape = (int)rnd(0, 3), wide = (int)rnd(0, 2);
    int nums[100], pool[10], all[10], count, i, j, t, lo, hi, len, d;

    for (i = 0; i < 10; i++)
        all[i] = i;
    rnd_shuffle(all, 10);
    count = (int)rnd(2, 4);
    for (i = 0; i < count; i++)
        pool[i] = all[i];
    if (pool[0] == 0) {
        pool[0] = pool[1];
        pool[1] = 0;
    }
    for (i = 0; i < n; i++) {
        len = (int)rnd(2, 5);
        if (shape == 0) {
            nums[i] = (int)rnd(10, 100000);
        } else if (shape == 1) {
            nums[i] = build(pool, count, len);
        } else if (shape == 2) {
            d = (int)rnd(1, 9);
            for (nums[i] = 0, j = 0; j < len; j++)
                nums[i] = nums[i] * 10 + d;
        } else if ((wide == 1 && i == n - 1) || (wide == 2 && i == 0)) {
            lo = (int)rnd(0, 4);
            hi = (int)rnd(lo + 4, 9);
            nums[i] = band(lo, hi, len);
        } else {
            lo = (int)rnd(0, 9);
            hi = (int)rnd(lo, lo + 3 > 9 ? 9 : lo + 3);
            nums[i] = band(lo, hi, len);
        }
    }
    if (shape != 2 && rnd_chance(20)) {
        i = (int)rnd(0, n - 1);
        nums[i] = rnd_chance(50) ? 100000 : 99999;
    }
    if (n > 1 && rnd_chance(30)) {
        i = (int)rnd(1, n - 1);
        j = (int)rnd(0, i - 1);
        t = nums[j];
        nums[i] = t;
    }
    put_ints(nums, n);
}
