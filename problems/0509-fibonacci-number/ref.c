/* Reference solution: build the list from the front, keeping only its
 * last two numbers. After k steps, a holds position k and b position
 * k + 1. */
int fib(int n)
{
    int a = 0, b = 1, i, next;

    for (i = 0; i < n; i++) {
        next = a + b;
        a = b;
        b = next;
    }
    return a;
}
