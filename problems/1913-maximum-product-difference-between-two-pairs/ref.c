/* Reference solution: every value is positive, so the best plus pair is
 * the two largest values and the best minus pair the two smallest; with at
 * least four entries these sit at four different positions. One pass keeps
 * the two largest and the two smallest values seen so far. The answer is
 * at most 10^4 * 10^4 - 1, which fits an int. */
int maxProductDifference(int *nums, int numsSize)
{
    int big1 = 0, big2 = 0, low1 = 10001, low2 = 10001, v, i;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (v > big1) {
            big2 = big1;
            big1 = v;
        } else if (v > big2) {
            big2 = v;
        }
        if (v < low1) {
            low2 = low1;
            low1 = v;
        } else if (v < low2) {
            low2 = v;
        }
    }
    return big1 * big2 - low1 * low2;
}
