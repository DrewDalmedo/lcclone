/* Reference solution: a number and the sum of its digits leave the same
 * remainder when divided by 9, so the last digit standing is num mod 9,
 * except that a positive multiple of 9 ends at 9, not 0. Taking 1 off
 * first, rather than adding 8, can't overflow near 2^31 - 1. */
int addDigits(int num)
{
    return num == 0 ? 0 : 1 + (num - 1) % 9;
}
