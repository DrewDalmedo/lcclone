/* Slow and obviously right: build the answer bit by bit. Bit b is set in
 * an OR exactly when it is set in at least one of the values combined, so
 * for each bit look for an even entry that has it. */
int evenNumberBitwiseORs(int *nums, int numsSize)
{
    int result = 0, b, i;

    for (b = 0; b < 31; b++)
        for (i = 0; i < numsSize; i++)
            if (nums[i] % 2 == 0 && (nums[i] >> b & 1))
                result |= 1 << b;
    return result;
}
