/* An array and three limits. Small cases have 3 to 8 values from a narrow
 * band (sometimes near 0, sometimes near 1000) and limits around the same
 * size, so that differences often equal a limit and values repeat. Full-size
 * ones have 100 values; the seed picks the shape: values from the whole
 * range with random limits, every limit 1000 (everything is good, and
 * nothing can be skipped), or values in a narrow band with small limits. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : 100, i, base, width, top;
    int shape = (int)rnd(0, 2), a, b, c;
    int arr[100];

    if (small) {
        width = (int)rnd(0, 6);
        base = rnd_chance(25) ? 1000 - width : rnd_chance(30) ? 0 : (int)rnd(0, 20);
        top = rnd_chance(10) ? 1000 : width + 1;
    } else if (shape == 0) {
        width = 1000;
        base = 0;
        top = 1000;
    } else if (shape == 1) {
        width = 1000;
        base = 0;
        top = -1;
    } else {
        width = (int)rnd(0, 30);
        base = (int)rnd(0, 1000 - width);
        top = width;
    }
    for (i = 0; i < n; i++)
        arr[i] = base + (int)rnd(0, width);
    if (top < 0) {
        a = b = c = 1000;
    } else {
        a = (int)rnd(0, top);
        b = (int)rnd(0, top);
        c = (int)rnd(0, top);
    }
    put_ints(arr, n);
    put_int(a);
    put_int(b);
    put_int(c);
}
