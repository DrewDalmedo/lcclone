/* Slow and obviously right: write out n's binary digits as characters
 * (just "0" for n = 0), then read them back from the highest one down,
 * each swapped. */
int bitwiseComplement(int n)
{
    char digits[32];
    int len = 0, result = 0, i;

    do {
        digits[len++] = (char)('0' + n % 2);
        n /= 2;
    } while (n > 0);
    for (i = len - 1; i >= 0; i--)
        result = result * 2 + (digits[i] == '0' ? 1 : 0);
    return result;
}
