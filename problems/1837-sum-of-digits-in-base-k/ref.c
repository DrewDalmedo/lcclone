/* Reference solution: the lowest base-k digit of n is n % k, and n / k
 * holds the remaining digits; add up the lowest digit until n is 0. */
int sumBase(int n, int k)
{
    int sum = 0;

    while (n > 0) {
        sum += n % k;
        n /= k;
    }
    return sum;
}
