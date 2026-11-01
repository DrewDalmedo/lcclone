/* Reference solution: never multiply. A zero makes the product zero;
 * otherwise each negative factor flips the sign, so keep a sign that
 * starts at 1 and flip it for every negative element. */
int arraySign(int *nums, int numsSize)
{
    int sign = 1, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] == 0)
            return 0;
        if (nums[i] < 0)
            sign = -sign;
    }
    return sign;
}
