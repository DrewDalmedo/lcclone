/* Slow and obviously right: go over 1..n, adding each number or
 * subtracting it when m divides it. */
int differenceOfSums(int n, int m)
{
    int x, result = 0;

    for (x = 1; x <= n; x++)
        if (x % m == 0)
            result -= x;
        else
            result += x;
    return result;
}
