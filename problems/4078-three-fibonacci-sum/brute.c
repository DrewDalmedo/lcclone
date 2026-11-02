/* Slow and obviously right: for i = 0, 1, 2, ..., work out F(i), F(i + 1)
 * and F(i + 2) from scratch, each by running the sequence from F(0), and
 * compare their sum with n, until the sums pass n. */
static long long fib(int k)
{
    long long a = 0, b = 1, next;
    int i;

    for (i = 0; i < k; i++) {
        next = a + b;
        a = b;
        b = next;
    }
    return a;
}

bool threeFibonacciSum(int n)
{
    long long sum;
    int i;

    for (i = 0;; i++) {
        sum = fib(i) + fib(i + 1) + fib(i + 2);
        if (sum == n)
            return true;
        if (sum > n)
            return false;
    }
}
