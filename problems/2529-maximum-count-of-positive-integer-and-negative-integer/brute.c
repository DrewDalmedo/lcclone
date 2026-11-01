/* Slow and obviously right: look at every value and count the negative
 * ones and the positive ones. */
int maximumCount(int *nums, int numsSize)
{
    int neg = 0, pos = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] < 0)
            neg++;
        else if (nums[i] > 0)
            pos++;
    }
    return neg > pos ? neg : pos;
}
