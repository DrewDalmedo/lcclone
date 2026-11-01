/* Reference solution: the common divisors of a and b are the divisors of
 * g = gcd(a, b). Count them in pairs d and g / d for d up to sqrt(g),
 * counting the square root once when g is a perfect square. */
int commonFactors(int a, int b)
{
    int g = a, r = b, t, d, count = 0;

    while (r != 0) {
        t = g % r;
        g = r;
        r = t;
    }
    for (d = 1; d * d <= g; d++)
        if (g % d == 0)
            count += d * d == g ? 1 : 2;
    return count;
}
