/* Slow and obviously right: try every x from 1 to the larger of a and b,
 * and count the ones that divide both. */
int commonFactors(int a, int b)
{
    int x, count = 0, top = a > b ? a : b;

    for (x = 1; x <= top; x++)
        if (a % x == 0 && b % x == 0)
            count++;
    return count;
}
