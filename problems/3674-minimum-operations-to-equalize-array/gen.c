/* The seed picks a shape: every entry equal (often 1 or 10^5); every
 * entry equal but one, at a random position; entries whose bits all
 * contain those of one base value, which is itself an entry (often the
 * first), so the AND of the row equals that entry; random values; or
 * random values with the last entry equal to the first. Small cases have
 * 1 to 8 entries up to 15; full-size ones have 100 entries up to 10^5. */
void gen(int small)
{
    int nums[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 15 : 100000;
    int bits = small ? 15 : 65535; /* base | extra stays within top */
    int shape = (int)rnd(0, 4), kind, i, v, w, p, extra;

    if (shape == 0) {
        kind = (int)rnd(0, 3);
        v = kind == 0 ? 1 : kind == 1 ? 100000 : (int)rnd(1, top);
        for (i = 0; i < n; i++)
            nums[i] = v;
    } else if (shape == 1) {
        v = (int)rnd(1, top);
        do
            w = (int)rnd(1, top);
        while (w == v);
        for (i = 0; i < n; i++)
            nums[i] = v;
        p = (int)rnd(0, n - 1);
        nums[p] = w;
    } else if (shape == 2) {
        v = (int)rnd(1, bits);
        for (i = 0; i < n; i++) {
            extra = (int)rnd(0, bits);
            nums[i] = v | extra;
        }
        p = rnd_chance(50) ? 0 : (int)rnd(0, n - 1);
        nums[p] = v;
    } else {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, top);
        if (shape == 4)
            nums[n - 1] = nums[0];
    }
    put_ints(nums, n);
}
