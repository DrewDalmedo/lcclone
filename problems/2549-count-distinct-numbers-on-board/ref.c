/* Reference solution: every x >= 3 leaves remainder 1 when divided by
 * x - 1, so the news runs down from n through every house to house 2.
 * House 2 tells nobody (2 mod i is 0 or 2), and house 1 is never told
 * (x mod 1 is always 0). So for n >= 2 houses 2 to n know it, n - 1 of
 * them; for n = 1 only house 1 does. */
int distinctIntegers(int n)
{
    return n == 1 ? 1 : n - 1;
}
