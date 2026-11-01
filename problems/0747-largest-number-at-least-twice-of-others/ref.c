/* Reference solution: one pass that keeps the index of the largest element
 * and the largest of the others (the runner-up). A new largest element
 * pushes the old one down to runner-up. Values are at least 0, so -1
 * means "no runner-up yet". Comparing with the runner-up is enough: the
 * other elements are no larger. */
int dominantIndex(int *nums, int numsSize)
{
    int top = 0, second = -1, i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] > nums[top]) {
            second = nums[top];
            top = i;
        } else if (nums[i] > second) {
            second = nums[i];
        }
    }
    return nums[top] >= 2 * second ? top : -1;
}
