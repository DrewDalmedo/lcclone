/* Reference solution: one pass, keeping the length of the climbing block
 * that ends at the current element: one more than for the element before
 * when this one is greater, else 1. The best length is updated at every
 * element, so a block that reaches the last element counts too. */
int findLengthOfLCIS(int *nums, int numsSize)
{
    int run = 1, best = 1, i;

    for (i = 1; i < numsSize; i++) {
        run = nums[i] > nums[i - 1] ? run + 1 : 1;
        if (run > best)
            best = run;
    }
    return best;
}
