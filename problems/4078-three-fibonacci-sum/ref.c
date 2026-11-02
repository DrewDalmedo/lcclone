/* Reference solution: F(i) + F(i + 1) = F(i + 2), so three neighbouring
 * terms add up to 2 * F(i + 2). n qualifies exactly when it is even and
 * n / 2 is one of F(2), F(3), ... = 1, 2, 3, 5, ...: walk those numbers
 * until one reaches n / 2. The largest one computed, for n near 10^9, is
 * F(45) = 1134903170, which still fits in an int. */
bool threeFibonacciSum(int n)
{
    int half = n / 2, a = 1, b = 2, next;

    if (n % 2 != 0)
        return false;
    while (a < half) {
        next = a + b;
        a = b;
        b = next;
    }
    return a == half;
}
