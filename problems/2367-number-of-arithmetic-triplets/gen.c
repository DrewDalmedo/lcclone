/* Small cases: 3 to 8 values, either picked from a lattice b + d * t (so
 * that ladders, overlapping ones and climbs of 2 * d come up often, with
 * diff = d, 2 * d or random), or from a narrow window with a small diff;
 * both often reach up to 200, where v + 2 * diff runs past the largest
 * value. Full-size cases, by the seed: every value 0..200 but one, with a
 * random diff; 0..199 or 1..200 with diff = 1 (the largest answer, 198);
 * or a random subset of 0..200. */
static void sort_ints(int *a, int n)
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
    int pick[201], nums[201];
    int n, d, top, base, width, diff, shape, gap, i;

    if (small) {
        n = (int)rnd(3, 8);
        if (rnd_chance(60)) {
            d = (int)rnd(1, 50);
            top = 200 / d < 9 ? 200 / d : 9;
            if (n > top + 1)
                n = top + 1;
            base = rnd_chance(40) ? 200 - d * top : (int)rnd(0, 200 - d * top);
            for (i = 0; i <= top; i++)
                pick[i] = i;
            rnd_shuffle(pick, top + 1);
            for (i = 0; i < n; i++)
                nums[i] = base + d * pick[i];
            if (rnd_chance(20) && 2 * d <= 50)
                diff = 2 * d;
            else if (rnd_chance(10))
                diff = (int)rnd(1, 50);
            else
                diff = d;
        } else {
            width = (int)rnd(n - 1, 12);
            base = rnd_chance(40) ? 200 - width : (int)rnd(0, 200 - width);
            for (i = 0; i <= width; i++)
                pick[i] = base + i;
            rnd_shuffle(pick, width + 1);
            for (i = 0; i < n; i++)
                nums[i] = pick[i];
            diff = (int)rnd(1, 4);
        }
    } else {
        shape = (int)rnd(0, 2);
        if (shape == 0) {
            gap = (int)rnd(0, 200);
            for (n = 0, i = 0; i <= 200; i++)
                if (i != gap)
                    nums[n++] = i;
            diff = (int)rnd(1, 50);
        } else if (shape == 1) {
            base = (int)rnd(0, 1);
            for (n = 200, i = 0; i < n; i++)
                nums[i] = base + i;
            diff = 1;
        } else {
            n = (int)rnd(3, 200);
            for (i = 0; i <= 200; i++)
                pick[i] = i;
            rnd_shuffle(pick, 201);
            for (i = 0; i < n; i++)
                nums[i] = pick[i];
            diff = (int)rnd(1, 50);
        }
    }
    sort_ints(nums, n);
    put_ints(nums, n);
    put_int(diff);
}
