/* Reference solution: walk the window from max(1, n - k) up to n + k and
 * add every x whose bitwise AND with n is zero. */
int sumOfGoodIntegers(int n, int k)
{
    int lo = n - k > 1 ? n - k : 1, x, sum = 0;

    for (x = lo; x <= n + k; x++)
        if ((n & x) == 0)
            sum += x;
    return sum;
}
