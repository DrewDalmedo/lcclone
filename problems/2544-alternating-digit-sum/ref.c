/* Reference solution: take the digits from the right with signs +, -, +,
 * ..., counting them. That gives the leading digit a + sign only when n
 * has an odd number of digits, so negate the sum when the count is even. */
int alternateDigitSum(int n)
{
    int sum = 0, sign = 1, count = 0;

    for (; n > 0; n /= 10) {
        sum += sign * (n % 10);
        sign = -sign;
        count++;
    }
    return count % 2 == 1 ? sum : -sum;
}
