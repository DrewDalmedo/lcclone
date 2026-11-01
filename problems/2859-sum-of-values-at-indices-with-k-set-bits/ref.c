/* Reference solution: count the 1 bits of each index i by clearing its
 * lowest 1 bit (i &= i - 1) until nothing is left, and add nums[i] when
 * the count is k. The total is at most 252 * 10^5, so an int holds it. */
int sumIndicesWithKSetBits(int *nums, int numsSize, int k)
{
    int sum = 0, i, bits, x;

    for (i = 0; i < numsSize; i++) {
        bits = 0;
        for (x = i; x != 0; x &= x - 1)
            bits++;
        if (bits == k)
            sum += nums[i];
    }
    return sum;
}
