/* Reference solution: reverse the lower half of the digits and compare
 * them with the upper half, which never overflows. */
bool isPalindrome(int x)
{
    int half = 0;

    if (x < 0 || (x % 10 == 0 && x != 0))
        return false;
    while (x > half) {
        half = half * 10 + x % 10;
        x /= 10;
    }
    return x == half || x == half / 10;
}
