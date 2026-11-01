/* Reference solution: peel off the digits with % 10 and / 10, keeping
 * their product (starting from 1) and their sum. */
int subtractProductAndSum(int n)
{
    int product = 1, sum = 0;

    while (n > 0) {
        product *= n % 10;
        sum += n % 10;
        n /= 10;
    }
    return product - sum;
}
