/* Slow and obviously right: list the powers of three 1, 3, 9, ... in 64
 * bits until one reaches n or passes it, and see whether it landed on n. */
bool isPowerOfThree(int n)
{
    long long p = 1;

    while (p < n)
        p *= 3;
    return p == n;
}
