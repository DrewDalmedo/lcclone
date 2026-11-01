/* Slow and obviously right: try every number from 1 to n and count the
 * ones that divide n. */
bool isThree(int n)
{
    int count = 0, d;

    for (d = 1; d <= n; d++)
        if (n % d == 0)
            count++;
    return count == 3;
}
