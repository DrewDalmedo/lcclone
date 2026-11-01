/* arr1, arr2 and d. Small cases have up to 8 entries in each array,
 * mostly from -8..8 with d from 0 to 4, so gaps of exactly d and d + 1
 * come up often; one in five uses values near -1000 and 1000 with d from
 * 90 to 100. Full-size cases have 500 entries in each array, in a shape
 * the seed picks:
 *   0: values from all of -1000..1000, d from 0 to 100;
 *   1: arr2 in a few tight clumps, arr1 spread over the whole range;
 *   2: arr2 drawn from a few far-apart points, and every arr1 entry
 *      exactly d or d + 1 above or below one of them;
 *   3: arr2 at one end of the range and arr1 at the other, so every
 *      entry keeps clear and a check must look at all of arr2. */
static int clamp(int v)
{
    return v < -1000 ? -1000 : v > 1000 ? 1000 : v;
}

static int near_end(void)
{
    int v = 1000 - (int)rnd(0, 120);

    return rnd_chance(50) ? v : -v;
}

void gen(int small)
{
    int arr1[500], arr2[500], point[20];
    int n1, n2, d, shape, wide, k, p, off, v, i, j;

    if (small) {
        n1 = (int)rnd(1, 8);
        n2 = (int)rnd(1, 8);
        wide = rnd_chance(20);
        d = wide ? (int)rnd(90, 100) : (int)rnd(0, 4);
        for (i = 0; i < n1; i++)
            arr1[i] = wide ? near_end() : (int)rnd(-8, 8);
        for (j = 0; j < n2; j++)
            arr2[j] = wide ? near_end() : (int)rnd(-8, 8);
        put_ints(arr1, n1);
        put_ints(arr2, n2);
        put_int(d);
        return;
    }
    n1 = n2 = 500;
    shape = (int)rnd(0, 3);
    d = (int)rnd(0, 100);
    if (shape == 0) {
        for (i = 0; i < n1; i++)
            arr1[i] = (int)rnd(-1000, 1000);
        for (j = 0; j < n2; j++)
            arr2[j] = (int)rnd(-1000, 1000);
    } else if (shape == 1) {
        k = (int)rnd(3, 8);
        for (i = 0; i < k; i++)
            point[i] = (int)rnd(-1000, 1000);
        for (j = 0; j < n2; j++) {
            v = point[rnd(0, k - 1)];
            arr2[j] = clamp(v + (int)rnd(-5, 5));
        }
        for (i = 0; i < n1; i++)
            arr1[i] = (int)rnd(-1000, 1000);
    } else if (shape == 2) {
        k = (int)rnd(3, 6);
        for (i = 0; i < k; i++)
            point[i] = -1000 + i * (2000 / (k - 1));
        for (j = 0; j < n2; j++)
            arr2[j] = point[rnd(0, k - 1)];
        for (i = 0; i < n1; i++) {
            v = arr2[rnd(0, n2 - 1)];
            off = rnd_chance(50) ? d : d + 1;
            if (rnd_chance(50))
                off = -off;
            if (v + off < -1000 || v + off > 1000)
                off = -off;
            arr1[i] = v + off;
        }
    } else {
        p = (int)rnd(-800, 800);
        if (rnd_chance(50)) {
            for (j = 0; j < n2; j++)
                arr2[j] = (int)rnd(-1000, p);
            for (i = 0; i < n1; i++)
                arr1[i] = (int)rnd(p + d + 1, 1000);
        } else {
            for (j = 0; j < n2; j++)
                arr2[j] = (int)rnd(p, 1000);
            for (i = 0; i < n1; i++)
                arr1[i] = (int)rnd(-1000, p - d - 1);
        }
    }
    put_ints(arr1, n1);
    put_ints(arr2, n2);
    put_int(d);
}
