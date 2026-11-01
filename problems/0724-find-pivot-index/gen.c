/* Random values, and often a pivot planted at an index p: the elements
 * after p are nudged, then if need be the ones before it, until the two
 * arms have the same sum. (An arm of k elements can reach any sum from
 * -1000k to 1000k, both ranges hold 0, so this always works.) An earlier
 * index may balance too; ref finds the smallest. Small cases have up to 8
 * values from [-3, 3] or [-1, 1], or from -1000, 1000 and a few values
 * around 0, with a pivot planted half of the time. Full-size cases have
 * 10^4 values, and the seed picks the shape: random values with nothing
 * planted (usually no pivot, the slowest for a method that adds up both
 * arms at every index); a pivot planted at the last index; one planted at
 * a random index; or 9999 copies of 1000, or of -1000, which balance at
 * index 4999 with arms of almost 5 * 10^6. */
static void nudge(int *a, int i, long long *diff, int up)
{
    long long room = up ? 1000 - a[i] : a[i] + 1000;
    long long need = *diff < 0 ? -*diff : *diff;
    long long step = need < room ? need : room;

    a[i] += (int)(up ? step : -step);
    *diff += *diff < 0 ? step : -step;
}

/* diff is the left arm minus the right arm: raise the right arm or lower
 * the left one while diff > 0, the other way round while diff < 0. */
static void plant(int *a, int n, int p)
{
    long long diff = 0;
    int i;

    for (i = 0; i < p; i++)
        diff += a[i];
    for (i = p + 1; i < n; i++)
        diff -= a[i];
    for (i = p + 1; i < n && diff != 0; i++)
        nudge(a, i, &diff, diff > 0);
    for (i = 0; i < p && diff != 0; i++)
        nudge(a, i, &diff, diff < 0);
}

void gen(int small)
{
    static const int wide[] = {-1000, 1000, -1, 0, 1};
    static int nums[10000];
    int n, i, kind, v, p, sign;

    if (small) {
        n = (int)rnd(1, 8);
        kind = (int)rnd(0, 2);
        for (i = 0; i < n; i++) {
            if (kind == 0)
                v = (int)rnd(-3, 3);
            else if (kind == 1)
                v = (int)rnd(-1, 1);
            else
                v = wide[rnd(0, 4)];
            nums[i] = v;
        }
        if (rnd_chance(50)) {
            p = (int)rnd(0, n - 1);
            plant(nums, n, p);
        }
        put_ints(nums, n);
        return;
    }

    n = 10000;
    kind = (int)(gen_seed() % 4);
    if (kind == 3) {
        n = 9999;
        sign = rnd_chance(50) ? 1 : -1;
        for (i = 0; i < n; i++)
            nums[i] = 1000 * sign;
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(-1000, 1000);
        if (kind == 1) {
            plant(nums, n, n - 1);
        } else if (kind == 2) {
            p = (int)rnd(0, n - 1);
            plant(nums, n, p);
        }
    }
    put_ints(nums, n);
}
