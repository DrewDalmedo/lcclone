/* Slow and obviously right, with arithmetic instead of bit operations:
 * write each element in base 2 by repeated division, tally the digits
 * place by place, then add up the powers of 2 whose tally reaches k. */
int findKOr(int *nums, int numsSize, int k)
{
    int tally[32] = {0};
    long long value, power = 1, result = 0;
    int i, place;

    for (i = 0; i < numsSize; i++) {
        value = nums[i];
        for (place = 0; value > 0; place++) {
            if (value % 2 == 1)
                tally[place]++;
            value /= 2;
        }
    }
    for (place = 0; place < 32; place++) {
        if (tally[place] >= k)
            result += power;
        power *= 2;
    }
    return (int)result;
}
