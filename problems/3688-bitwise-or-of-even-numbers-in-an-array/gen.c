/* One random array with a chosen number of even entries: none, one, all,
 * or any number (two times in five). A lone even entry is put last half
 * the time; otherwise the entries are shuffled. Small cases have 1 to 8
 * entries, mostly of at most 20, so that repeated values and evens whose
 * bits overlap come up often; full-size ones have 100 entries from 1 to
 * 100. */
static int pick(int even, int top)
{
    if (even)
        return 2 * (int)rnd(1, top / 2);
    return 2 * (int)rnd(1, (top + 1) / 2) - 1;
}

void gen(int small)
{
    int nums[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small && rnd_chance(70) ? 20 : 100;
    int shape = (int)rnd(0, 4), evens, t, i;

    evens = shape == 0 ? 0 : shape == 1 ? 1 : shape == 2 ? n
          : (int)rnd(0, n);
    for (i = 0; i < n; i++)
        nums[i] = pick(i < evens, top);
    if (shape == 1 && rnd_chance(50)) {
        t = nums[0];
        nums[0] = nums[n - 1];
        nums[n - 1] = t;
    } else {
        rnd_shuffle(nums, n);
    }
    put_ints(nums, n);
}
