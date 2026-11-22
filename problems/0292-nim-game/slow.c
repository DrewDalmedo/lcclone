/* Right, but too slow at full size: work out every pile from 3 up to n
 * from the three before it, keeping only those three. It is a tight loop,
 * far faster than brute.c, but still about 2 * 10^9 steps for the largest
 * piles. */
bool canWinNim(int n)
{
    bool a = false, b = true, c = true, d; /* piles 0, 1 and 2 */
    int i;

    if (n <= 2)
        return n > 0;
    for (i = 3; i <= n; i++) {
        d = !a || !b || !c;
        a = b;
        b = c;
        c = d;
    }
    return c;
}
