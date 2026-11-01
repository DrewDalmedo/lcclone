/* Reference solution: an entry is even and divisible by 3 exactly when it
 * is a multiple of 6. Add those up and count them in one pass, then divide
 * (integer division rounds the positive mean down), guarding against an
 * empty selection. */
int averageValue(int *nums, int numsSize)
{
    int sum = 0, count = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] % 6 == 0) {
            sum += nums[i];
            count++;
        }
    return count ? sum / count : 0;
}
