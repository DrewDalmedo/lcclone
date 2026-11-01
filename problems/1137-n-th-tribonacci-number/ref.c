/* Reference solution: walk up the sequence keeping only the last three
 * terms, and stop at term n. No term past T(n) is ever computed, which
 * matters at the top: T(38) would overflow an int. */
int tribonacci(int n)
{
    int a = 0, b = 1, c = 1, next, i; /* T(i-2), T(i-1), T(i) for i = 2 */

    if (n < 2)
        return n;
    for (i = 3; i <= n; i++) {
        next = a + b + c;
        a = b;
        b = c;
        c = next;
    }
    return c;
}
