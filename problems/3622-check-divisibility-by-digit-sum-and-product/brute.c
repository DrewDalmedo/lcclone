/* Slow and obviously right: write n out as text, read the digits back
 * as characters, and look for t among the divisors of n one by one. */
bool checkDivisibility(int n)
{
    char text[16];
    int sum = 0, prod = 1, t, d, i;

    snprintf(text, sizeof text, "%d", n);
    for (i = 0; text[i] != '\0'; i++) {
        sum += text[i] - '0';
        prod *= text[i] - '0';
    }
    t = sum + prod;
    for (d = 1; d <= n; d++)
        if (n % d == 0 && d == t)
            return true;
    return false;
}
