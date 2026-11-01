/* Reference solution: two neighbours differ in parity exactly when the
 * lowest bit of their XOR is set; check every neighbouring pair. */
bool isArraySpecial(int *nums, int numsSize)
{
    int i;

    for (i = 0; i + 1 < numsSize; i++)
        if (((nums[i] ^ nums[i + 1]) & 1) == 0)
            return false;
    return true;
}
