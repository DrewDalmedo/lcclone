/* Slow and obviously right: the definition as a recursion, with nothing
 * remembered. About 2.7 million calls at n = 30. */
int fib(int n)
{
    return n < 2 ? n : fib(n - 1) + fib(n - 2);
}
