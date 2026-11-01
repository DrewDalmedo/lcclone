/* Reference solution: in closed form. The numbers 1..n add up to
 * n(n+1)/2; the q = n/m multiples of m among them are m, 2m, ..., qm,
 * which add up to m * q(q+1)/2. Those are subtracted instead of added,
 * so they come off the total twice. */
int differenceOfSums(int n, int m)
{
    int total = n * (n + 1) / 2, q = n / m;
    int multiples = m * (q * (q + 1) / 2);

    return total - 2 * multiples;
}
