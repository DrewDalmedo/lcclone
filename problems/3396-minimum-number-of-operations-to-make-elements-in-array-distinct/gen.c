/* The seed picks the shape: codes drawn from 1 to k, so repeats are common
 * (k small in small cases, any up to 100 in full-size ones); different
 * codes with one repeat planted at a random pair of positions, or at the
 * last two (the slowest case for a sweep-by-sweep check), sometimes with an
 * earlier repeat overlapping it; different codes only; or a single code
 * throughout. Small cases have 1 to 10 parcels; full-size ones mostly 100,
 * with codes up to 100. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : rnd_chance(75) ? 100 : (int)rnd(1, 100);
    int shape = (int)rnd(0, 4), top = small ? 12 : 100;
    int vals[100], nums[100], i, k, p, q, p2, q2, v;

    for (i = 0; i < top; i++)
        vals[i] = i + 1;
    rnd_shuffle(vals, top);
    if (shape <= 1) {
        k = small ? (int)rnd(1, n + 1) : (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            nums[i] = vals[rnd(0, k - 1)];
    } else {
        for (i = 0; i < n; i++)
            nums[i] = vals[i];
        if (shape == 4) {
            v = (int)rnd(1, 100);
            for (i = 0; i < n; i++)
                nums[i] = v;
        }
    }
    if (shape == 2 && n >= 2) {
        if (rnd_chance(30)) {
            p = n - 2;
            q = n - 1;
        } else {
            p = (int)rnd(0, n - 2);
            q = (int)rnd(p + 1, n - 1);
        }
        nums[q] = nums[p];
        if (p >= 1 && n >= 4 && rnd_chance(50)) {
            /* An earlier repeat, whose second copy may come after q. */
            p2 = (int)rnd(0, p - 1);
            q2 = (int)rnd(p2 + 1, n - 1);
            if (q2 != p && q2 != q)
                nums[q2] = nums[p2];
        }
    }
    put_ints(nums, n);
}
