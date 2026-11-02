/* Reference solution: build the mirror image digit by digit from the low
 * end of n. Only 10^9 has ten digits, and its mirror image is 1, so the
 * mirror image always fits in an int, and so does the distance. */
int mirrorDistance(int n)
{
    int m = 0, x = n;

    while (x > 0) {
        m = m * 10 + x % 10;
        x /= 10;
    }
    return n > m ? n - m : m - n;
}
