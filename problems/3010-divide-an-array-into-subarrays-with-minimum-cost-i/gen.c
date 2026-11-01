/* Small cases: 3 to 8 numbers from 1..t (t is 2 to 9), with the first one
 * now and then 1 or 50, so that equal smallest numbers in the tail, a
 * first number below or above all the others, and the smallest numbers at
 * position 1 or at the end come up often.
 * Full-size cases: 50 numbers, in a shape the seed picks:
 *   0: numbers from all of 1..50;
 *   1: a falling tail, from 50 down in steps of 0 or 1, so the two
 *      smallest are at the end;
 *   2: nums[0] is 50 and the rest come from 1..50;
 *   3: the two smallest at position 1 and at the end, the rest larger;
 *   4: every number is 50 (the answer is 150). */
void gen(int small)
{
    int shape = small ? -1 : (int)(gen_seed() % 5);
    int n = small ? (int)rnd(3, 8) : 50;
    int t = small ? (int)rnd(2, 9) : 50;
    int nums[50], i, k, lo, v;

    for (i = 0; i < n; i++)
        nums[i] = (int)rnd(1, t);
    if (shape == -1) {
        k = (int)rnd(0, 3);
        if (k == 0)
            nums[0] = 1;
        else if (k == 1)
            nums[0] = 50;
    } else if (shape == 1) {
        v = 50;
        for (i = 1; i < n; i++) {
            nums[i] = v;
            v -= (int)rnd(0, 1);
        }
    } else if (shape == 2) {
        nums[0] = 50;
    } else if (shape == 3) {
        lo = (int)rnd(1, 24);
        v = (int)rnd(lo, 24);
        for (i = 2; i < n - 1; i++)
            nums[i] = (int)rnd(v + 1, 50);
        k = (int)rnd(0, 1);
        nums[1] = k ? lo : v;
        nums[n - 1] = k ? v : lo;
    } else if (shape == 4) {
        for (i = 0; i < n; i++)
            nums[i] = 50;
    }
    put_ints(nums, n);
}
