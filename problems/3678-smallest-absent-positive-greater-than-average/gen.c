/* One random array. The seed picks the shape: values from -top to top;
 * positive values only, so that values present just above the average are
 * common; pairs c - d and c + d around a centre c, so that the average is
 * the whole number c and c is missing; a run of consecutive values; mostly
 * zero or negative values; or every entry equal. Full-size cases have 100
 * entries from -100 to 100, and one more shape there: 1 to 100 in random
 * order, whose answer, 101, takes the most steps. Small cases have 1 to 8
 * entries, mostly of size at most 10, so that ties and repeats come up
 * often. */
void gen(int small)
{
    int nums[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 10 : 100;
    int shape = (int)rnd(0, small ? 5 : 6);
    int c, d, lim, a, v, i;

    if (shape == 2 && n % 2)
        n = n < 8 ? n + 1 : n - 1;
    if (shape == 2) {
        c = (int)rnd(1, small ? 6 : 99);
        lim = top - c < c + top ? top - c : c + top;
        for (i = 0; i < n; i += 2) {
            d = (int)rnd(1, lim);
            nums[i] = c - d;
            nums[i + 1] = c + d;
        }
    } else if (shape == 3) {
        a = small ? (int)rnd(-3, 6) : (int)rnd(-100, 1);
        for (i = 0; i < n; i++)
            nums[i] = a + i;
    } else if (shape == 5) {
        v = small ? (int)rnd(-3, top)
                  : rnd_chance(50) ? 100 : (int)rnd(-100, 100);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 6) {
        for (i = 0; i < n; i++)
            nums[i] = i + 1;
    } else {
        for (i = 0; i < n; i++)
            nums[i] = shape == 0 ? (int)rnd(-top, top)
                    : shape == 1 ? (int)rnd(1, top)
                    : (int)rnd(-top, 2);
    }
    rnd_shuffle(nums, n);
    put_ints(nums, n);
}
