/* Slow and obviously right: the definition as a recursion, with no memory
 * of terms already worked out, so it takes exponential time. */
int tribonacci(int n)
{
    if (n == 0)
        return 0;
    if (n <= 2)
        return 1;
    return tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);
}
