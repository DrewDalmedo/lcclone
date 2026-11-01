/* Reference solution: for each number, walk its digits to find the highest
 * one, and build the encrypted number by repeating that digit once per
 * digit, zeros included, with integer arithmetic. */
int sumOfEncryptedInt(int *nums, int numsSize)
{
    int sum = 0, i, x, high, rep;

    for (i = 0; i < numsSize; i++) {
        high = 0;
        rep = 0;
        for (x = nums[i]; x > 0; x /= 10) {
            if (x % 10 > high)
                high = x % 10;
            rep = rep * 10 + 1;
        }
        sum += high * rep;
    }
    return sum;
}
