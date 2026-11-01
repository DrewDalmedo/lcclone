/* Slow and obviously right: work out the exact product as a big number,
 * its magnitude in base-10000 digits (least significant first) and its
 * sign, then read off the sign. 1000 factors of at most 100 have at most
 * 2001 decimal digits, so 520 base-10000 digits are plenty. */
int arraySign(int *nums, int numsSize)
{
    static int digit[520];
    int len = 1, negative = 0, i, d, carry, mag, zero = 1;

    digit[0] = 1;
    for (i = 0; i < numsSize; i++) {
        mag = nums[i] < 0 ? -nums[i] : nums[i];
        if (nums[i] < 0)
            negative = !negative;
        carry = 0;
        for (d = 0; d < len; d++) {
            carry += digit[d] * mag;
            digit[d] = carry % 10000;
            carry /= 10000;
        }
        while (carry) {
            digit[len++] = carry % 10000;
            carry /= 10000;
        }
    }
    for (d = 0; d < len; d++)
        if (digit[d] != 0)
            zero = 0;
    if (zero)
        return 0;
    return negative ? -1 : 1;
}
