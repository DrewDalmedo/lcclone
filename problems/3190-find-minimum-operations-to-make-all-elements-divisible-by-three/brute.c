/* Slow and obviously right: for each element, try distances 0, 1, 2, ...
 * until moving that far up or down (not below 0) lands on a multiple of
 * 3, and add up those distances. */
int minimumOperations(int *nums, int numsSize)
{
    int total = 0, i, d;

    for (i = 0; i < numsSize; i++) {
        for (d = 0;; d++) {
            if ((nums[i] + d) % 3 == 0)
                break;
            if (nums[i] - d >= 0 && (nums[i] - d) % 3 == 0)
                break;
        }
        total += d;
    }
    return total;
}
