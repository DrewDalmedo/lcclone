/* Reference solution: take the digits off the low end with % 10, and put
 * each non-zero one in front of the digits kept so far, at the next place
 * value. Everything is 64-bit: n and the result reach 10^15. */
long long removeZeros(long long n)
{
    long long result = 0, place = 1;

    while (n > 0) {
        int digit = (int)(n % 10);

        if (digit != 0) {
            result += digit * place;
            place *= 10;
        }
        n /= 10;
    }
    return result;
}
