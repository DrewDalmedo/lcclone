/* Reference solution: one pass from left to right, remembering where the
 * latest 1 and the latest 2 were seen. The closest partner on the left
 * of each 1 or 2 is the latest one of the other kind, and every pair is
 * met this way from its right-hand end. */
int minAbsoluteDifference(int *nums, int numsSize)
{
    int last1 = -1, last2 = -1, best = -1, i, d;

    for (i = 0; i < numsSize; i++) {
        d = -1;
        if (nums[i] == 1) {
            last1 = i;
            if (last2 >= 0)
                d = i - last2;
        } else if (nums[i] == 2) {
            last2 = i;
            if (last1 >= 0)
                d = i - last1;
        }
        if (d >= 0 && (best < 0 || d < best))
            best = d;
    }
    return best;
}
