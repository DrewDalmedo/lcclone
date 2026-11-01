/* Reference solution: the best product is either the three largest values
 * multiplied together, or the two smallest (two negatives of large size
 * make a large positive) times the largest. One pass keeps the three
 * largest values and the two smallest, counting repeated values
 * separately. The starting values lie outside the range of nums, and
 * every one of them is replaced, since nums has at least three elements.
 * Every product of three values is within +-10^9, so int is enough. */
int maximumProduct(int *nums, int numsSize)
{
    int max1 = -1001, max2 = -1001, max3 = -1001;
    int min1 = 1001, min2 = 1001;
    int i, v, a, b;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (v > max1) {
            max3 = max2;
            max2 = max1;
            max1 = v;
        } else if (v > max2) {
            max3 = max2;
            max2 = v;
        } else if (v > max3) {
            max3 = v;
        }
        if (v < min1) {
            min2 = min1;
            min1 = v;
        } else if (v < min2) {
            min2 = v;
        }
    }
    a = max1 * max2 * max3;
    b = min1 * min2 * max1;
    return a > b ? a : b;
}
