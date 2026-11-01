/* Reference solution: one pass, adding each element with a sign that
 * starts at + and flips after every element. */
int alternatingSum(int *nums, int numsSize)
{
    int total = 0, sign = 1, i;

    for (i = 0; i < numsSize; i++) {
        total += sign * nums[i];
        sign = -sign;
    }
    return total;
}
