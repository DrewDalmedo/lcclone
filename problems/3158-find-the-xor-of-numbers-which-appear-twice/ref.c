/* Reference solution: one pass with a 64-bit mask of the values seen so
 * far (every value is at most 50, so 1ULL << v is safe). Meeting a value
 * whose bit is already set means this is its second copy: XOR it into the
 * answer then, so each doubled value counts once. */
int duplicateNumbersXOR(int *nums, int numsSize)
{
    unsigned long long seen = 0;
    int result = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (seen & (1ULL << nums[i]))
            result ^= nums[i];
        else
            seen |= 1ULL << nums[i];
    }
    return result;
}
