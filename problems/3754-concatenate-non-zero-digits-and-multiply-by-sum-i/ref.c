/* Reference solution: take the digits off the low end with % 10. Each
 * non-zero one goes in front of the digits kept so far, at the next place
 * value, and adds to the digit sum. x * s reaches about 8 * 10^10, so the
 * product is formed in 64 bits. */
long long sumAndMultiply(int n)
{
    long long x = 0, place = 1;
    int s = 0;

    while (n > 0) {
        int digit = n % 10;

        if (digit != 0) {
            x += digit * place;
            place *= 10;
            s += digit;
        }
        n /= 10;
    }
    return x * s;
}
