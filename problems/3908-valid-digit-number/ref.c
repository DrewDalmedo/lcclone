/* Reference solution: peel digits off the right while more than one is
 * left, noting whether any of them is x; the one left over is the leading
 * digit, which must not be x. */
bool validDigit(int n, int x)
{
    bool seen = false;

    while (n >= 10) {
        if (n % 10 == x)
            seen = true;
        n /= 10;
    }
    return seen && n != x;
}
