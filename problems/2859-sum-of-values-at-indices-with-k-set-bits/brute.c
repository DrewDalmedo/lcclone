/* Slow and obviously right: write each index in binary as a string of
 * digits, by repeated division by 2, then count the '1' characters. */
int sumIndicesWithKSetBits(int *nums, int numsSize, int k)
{
    char digits[40];
    long long sum = 0;
    int i, len, ones, x, d;

    for (i = 0; i < numsSize; i++) {
        len = 0;
        x = i;
        do {
            digits[len++] = (char)('0' + x % 2);
            x /= 2;
        } while (x > 0);
        ones = 0;
        for (d = 0; d < len; d++)
            if (digits[d] == '1')
                ones++;
        if (ones == k)
            sum += nums[i];
    }
    return (int)sum;
}
