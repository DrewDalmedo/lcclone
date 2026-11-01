/* Small cases have 1 to 6 numbers in each array; full-size ones 50 in
 * each. The seed picks k (1, 2 to 5, any of 1..50, or 1 to 3) and how
 * nums2 is drawn (from 1..6, 1..10 or all of 1..50). Each number of nums1
 * is one of these, for a random j: a multiple of nums2[j] * k (a good
 * pair); such a multiple plus 1 to k - 1, which rounds down to a multiple
 * after dividing by k (a trap for truncating division); a multiple of
 * nums2[j] below nums2[j] * k (the wrong way round); or any number of
 * 1..top, where top is 12 or 50. */

static int plank(const int *nums2, int m, int k, int top)
{
    int pick = (int)rnd(0, 3);
    int j = (int)rnd(0, m - 1);
    int step = nums2[j] * k, r, most;

    if (pick == 0 && step <= 50) {
        most = 50 / step;
        return step * (int)rnd(1, most);
    }
    if (pick == 1 && k >= 2) {
        r = (int)rnd(1, k - 1);
        if (step + r <= 50) {
            most = (50 - r) / step;
            return step * (int)rnd(1, most) + r;
        }
    }
    if (pick == 2 && k >= 2) {
        most = 50 / nums2[j] < k - 1 ? 50 / nums2[j] : k - 1;
        return nums2[j] * (int)rnd(1, most);
    }
    return (int)rnd(1, top);
}

void gen(int small)
{
    int nums1[50], nums2[50];
    int shape = (int)rnd(0, 3);
    int n = small ? (int)rnd(1, 6) : 50;
    int m = small ? (int)rnd(1, 6) : 50;
    int k, top1, top2, i;

    if (shape == 0) {
        k = 1;
        top2 = 10;
    } else if (shape == 1) {
        k = (int)rnd(2, 5);
        top2 = 6;
    } else if (shape == 2) {
        k = (int)rnd(1, 50);
        top2 = 50;
    } else {
        k = (int)rnd(1, 3);
        top2 = small ? 6 : 50;
    }
    top1 = rnd_chance(50) ? 12 : 50;
    for (i = 0; i < m; i++)
        nums2[i] = (int)rnd(1, top2);
    for (i = 0; i < n; i++)
        nums1[i] = plank(nums2, m, k, top1);
    put_ints(nums1, n);
    put_ints(nums2, m);
    put_int(k);
}
