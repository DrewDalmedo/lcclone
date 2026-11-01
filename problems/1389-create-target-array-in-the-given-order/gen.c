/* nums, then index with 0 <= index[i] <= i. Small cases have up to 8
 * numbers, mostly from 0..5 (now and then 100), so values repeat; index
 * is random, all 0 (always at the left end) or all i (always at the right
 * end). Full-size cases have 100 numbers from 0..100, with index in a
 * shape the seed picks:
 *   0: random from 0..i;
 *   1: always 0, so every step moves the whole row;
 *   2: mostly i (append), with an occasional slot at the far left;
 *   3: near the middle of the row;
 *   4: one fixed position p (or the end, while the row is shorter), so
 *      many numbers land on the same spot. */
void gen(int small)
{
    int nums[100], index[100];
    int n, shape, p, v, i;

    n = small ? (int)rnd(1, 8) : 100;
    shape = small ? (int)rnd(0, 5) : (int)rnd(0, 4);
    p = (int)rnd(0, 20);
    for (i = 0; i < n; i++) {
        if (small)
            nums[i] = rnd_chance(10) ? 100 : (int)rnd(0, 5);
        else
            nums[i] = (int)rnd(0, 100);
    }
    for (i = 0; i < n; i++) {
        if (small) {
            if (shape == 4)
                index[i] = 0;
            else if (shape == 5)
                index[i] = i;
            else
                index[i] = (int)rnd(0, i);
        } else if (shape == 0) {
            index[i] = (int)rnd(0, i);
        } else if (shape == 1) {
            index[i] = 0;
        } else if (shape == 2) {
            index[i] = rnd_chance(10) ? 0 : i;
        } else if (shape == 3) {
            v = i / 2 + (int)rnd(-2, 2);
            index[i] = v < 0 ? 0 : v > i ? i : v;
        } else {
            index[i] = p < i ? p : i;
        }
    }
    put_ints(nums, n);
    put_ints(index, n);
}
