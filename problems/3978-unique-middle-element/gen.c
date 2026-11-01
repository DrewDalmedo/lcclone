/* The seed picks a shape: values from a narrow range, so repeats are
 * common (all equal when the range is a single value); all values
 * different, so the centre is alone; all different but for one copy of
 * the centre's value, at the start, at the end, beside the centre or
 * anywhere; or all different but for a copy of the value just after (or
 * before) the centre, which stays alone. The all-different shapes put
 * 100 at the centre now and then. Small cases have 1 to 7 entries drawn
 * from 1 to 8 and 100; full-size ones have 99, the longest odd length,
 * drawn from 1 to 100. */
void gen(int small)
{
    int n = small ? 2 * (int)rnd(0, 3) + 1 : 99;
    int mid = n / 2, shape = (int)rnd(0, 3);
    int pool[100], size = small ? 9 : 100;
    int i, top, where, p, from, t;

    if (shape == 0) {
        top = (int)rnd(1, small ? 4 : 8);
        for (i = 0; i < n; i++)
            pool[i] = (int)rnd(1, top);
        put_ints(pool, n);
        return;
    }
    for (i = 0; i < size; i++)
        pool[i] = i + 1;
    if (small)
        pool[8] = 100;
    rnd_shuffle(pool, size);
    if (rnd_chance(30)) {
        for (i = 0; pool[i] != 100; i++)
            ;
        t = pool[i];
        pool[i] = pool[mid];
        pool[mid] = t;
    }
    if (shape == 2 && n > 1) {
        where = (int)rnd(0, 3);
        if (where == 0)
            p = 0;
        else if (where == 1)
            p = n - 1;
        else if (where == 2)
            p = rnd_chance(50) ? mid - 1 : mid + 1;
        else
            p = (int)rnd(0, n - 2);
        if (p == mid)
            p = n - 1;
        pool[p] = pool[mid];
    } else if (shape == 3 && n >= 3) {
        from = rnd_chance(50) ? mid + 1 : mid - 1;
        p = (int)rnd(0, n - 1);
        if (p == mid || p == from)
            p = from == mid + 1 ? 0 : n - 1;
        pool[p] = pool[from];
    }
    put_ints(pool, n);
}
