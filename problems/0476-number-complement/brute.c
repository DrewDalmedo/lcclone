/* Slow and obviously right: write num's binary digits into an array by
 * dividing by 2 over and over, then read them back from the highest one
 * down, each flipped. */
int findComplement(int num)
{
    int digits[32], n = 0, result = 0, i;

    while (num > 0) {
        digits[n++] = num % 2;
        num /= 2;
    }
    for (i = n - 1; i >= 0; i--)
        result = result * 2 + (1 - digits[i]);
    return result;
}
