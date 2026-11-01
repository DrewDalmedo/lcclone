/* An array that starts with a run of len values counting up by one, with
 * sum s, followed by an element that breaks the run. When s is small
 * enough, the rest often holds s, s + 1, ..., s + c - 1 in shuffled order
 * (and sometimes not s + c), so that the answer lies past a chain of
 * present values; the rest is random. Small cases have 1 to 8 values of at
 * most 12 or 50. Full-size ones have 50 values, and the seed picks the
 * shape: the whole array 1, 2, ..., 50 (s = 1275), a run of 2 to 12 values
 * (s often above 50), a run of 1 to 3 values with a long chain after it,
 * or a run of a single value. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(1, 8) : 50;
    int top = small && rnd_chance(50) ? 12 : 50;
    int shape = (int)rnd(0, 3), len, start, s = 0, c = 0, avoid, i, j, t, v;

    if (shape == 0)
        len = small ? (int)rnd(1, n) : n;
    else if (shape == 1)
        len = small ? (int)rnd(1, n) : (int)rnd(2, 12);
    else if (shape == 2)
        len = (int)rnd(1, n < 3 ? n : 3);
    else
        len = 1;
    start = (int)rnd(1, top - len + 1);
    for (i = 0; i < len; i++) {
        nums[i] = start + i;
        s += start + i;
    }
    if (s <= top && rnd_chance(70)) {
        t = top - s + 1 < n - len ? top - s + 1 : n - len;
        c = (int)rnd(0, t);
    }
    for (i = 0; i < c; i++)
        nums[len + i] = s + i;
    avoid = rnd_chance(50) && s + c <= top;
    for (i = len + c; i < n; i++) {
        do
            v = (int)rnd(1, top);
        while (avoid && v == s + c);
        nums[i] = v;
    }
    rnd_shuffle(nums + len, n - len);
    /* Make sure the element after the run breaks it. */
    if (len < n && nums[len] == nums[len - 1] + 1) {
        for (j = len + 1; j < n && nums[j] == nums[len - 1] + 1; j++)
            ;
        if (j < n) {
            t = nums[len];
            nums[len] = nums[j];
            nums[j] = t;
        } else {
            nums[len] = nums[len - 1];
        }
    }
    put_ints(nums, n);
}
