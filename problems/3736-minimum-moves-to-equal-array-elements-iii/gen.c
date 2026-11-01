/* Tower heights. Small cases have up to 8 towers, and the seed picks the
 * shape: heights of at most 4 (ties and equal rows are common), heights
 * near 100, all the same height, or any heights from 1 to 100.
 * Full-size ones have 100 towers: any heights, one tower of 100 among
 * towers of 1 (the largest answer, 9801), all the same height, or towers
 * of 100 with a few shorter ones. */
void gen(int small)
{
    int nums[100], n, shape, same, v = 1, i;

    shape = (int)rnd(0, 3);
    n = small ? (int)rnd(1, 8) : 100;
    if (shape == 2) {
        same = (int)rnd(0, 2);
        v = same == 0 ? 1 : same == 1 ? 100 : (int)rnd(1, 100);
    }
    for (i = 0; i < n; i++) {
        if (shape == 2) {
            nums[i] = v;
        } else if (shape == 0 && small) {
            nums[i] = (int)rnd(1, 4);
        } else if (shape == 1) {
            nums[i] = small ? (int)rnd(95, 100) : 1;
        } else if (shape == 3 && !small) {
            v = (int)rnd(1, 100);
            nums[i] = rnd_chance(5) ? v : 100;
        } else {
            nums[i] = (int)rnd(1, 100);
        }
    }
    if (!small && shape == 1)
        nums[rnd(0, n - 1)] = 100;
    put_ints(nums, n);
}
