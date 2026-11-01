/* Slow and obviously right: fill the rows one at a time, taking r coins
 * for row r, while enough coins are left to fill it. Only subtractions,
 * so nothing overflows. */
int arrangeCoins(int n)
{
    int rows = 0;

    while (n >= rows + 1) {
        rows++;
        n -= rows;
    }
    return rows;
}
