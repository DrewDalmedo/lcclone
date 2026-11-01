/* Slow and obviously right: try k = 1, 2, 3, ... until k * k (in 64 bits)
 * reaches num, and see whether it landed on num. */
bool isPerfectSquare(int num)
{
    long long k = 1;

    while (k * k < num)
        k++;
    return k * k == num;
}
