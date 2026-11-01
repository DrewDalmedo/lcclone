/* Reference solution: above 333, 3 * n has four digits and the row is too
 * long. Otherwise n, 2 * n and 3 * n have three digits each; take them one
 * by one, without building the row, and fail on a 0 or on a digit already
 * seen. Nine digits with no 0 and no repeat are exactly 1 to 9. */
bool isFascinating(int n)
{
    int seen = 0, k, m, d;

    if (n > 333)
        return false;
    for (k = 1; k <= 3; k++)
        for (m = k * n; m > 0; m /= 10) {
            d = m % 10;
            if (d == 0 || (seen & (1 << d)))
                return false;
            seen |= 1 << d;
        }
    return true;
}
