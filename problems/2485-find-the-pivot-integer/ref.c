/* Reference solution: with T = 1 + 2 + ... + n, the totals below and above
 * rung x are x(x + 1)/2 and T - x(x - 1)/2, which are equal exactly when
 * x * x == T. So take the integer square root of T and check it. */
int pivotInteger(int n)
{
    int total = n * (n + 1) / 2;
    int x = (int)sqrt((double)total);

    while (x * x > total)
        x--;
    while ((x + 1) * (x + 1) <= total)
        x++;
    return x * x == total ? x : -1;
}
