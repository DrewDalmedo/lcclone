/* Small cases: 1 to 8 elements from 1 to 6, and a target from 1 to 7 that
 * is often missing. Full-size cases have 100 elements; the seed picks
 * random values with a target taken from them or from 1 to 100, few
 * distinct values, every element equal to the target, or the target as
 * the largest or the smallest element. */
void gen(int small)
{
    int nums[100], n = small ? (int)rnd(1, 8) : 100, shape, target, v, i;

    if (small) {
        for (i = 0; i < n; i++)
            nums[i] = (int)rnd(1, 6);
        target = (int)rnd(1, 7);
        put_ints(nums, n);
        put_int(target);
        return;
    }
    shape = (int)rnd(0, 4);
    for (i = 0; i < n; i++) {
        if (shape == 1)
            v = (int)rnd(40, 45);
        else if (shape == 3)
            v = (int)rnd(1, 99);
        else if (shape == 4)
            v = (int)rnd(2, 100);
        else
            v = (int)rnd(1, 100);
        nums[i] = v;
    }
    if (shape == 2) {
        target = (int)rnd(1, 100);
        for (i = 0; i < n; i++)
            nums[i] = target;
    } else if (shape == 3 || shape == 4) {
        target = shape == 3 ? 100 : 1;
        for (i = 0; i < 5; i++) {
            v = (int)rnd(0, n - 1);
            nums[v] = target;
        }
    } else if (rnd_chance(75)) {
        v = (int)rnd(0, n - 1);
        target = nums[v];
    } else {
        target = (int)rnd(1, 100);
    }
    put_ints(nums, n);
    put_int(target);
}
