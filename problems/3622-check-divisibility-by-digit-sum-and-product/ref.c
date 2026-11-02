/* Reference solution: peel the digits off with % 10, keeping their sum
 * and their product (a 0 digit makes the product 0), then test whether
 * sum + product divides n. */
bool checkDivisibility(int n)
{
    int m = n, sum = 0, prod = 1;

    while (m > 0) {
        sum += m % 10;
        prod *= m % 10;
        m /= 10;
    }
    return n % (sum + prod) == 0;
}
