/* Reference solution: inclusion-exclusion. The multiples of d up to n add
 * up to d * (1 + 2 + ... + m) with m = n / d. Add the sums for 3, 5 and 7,
 * take away those for 15, 21 and 35 (counted twice), and add back the one
 * for 105 (taken away once too often). */
static int multiples_sum(int n, int d)
{
    int m = n / d;

    return d * m * (m + 1) / 2;
}

int sumOfMultiples(int n)
{
    return multiples_sum(n, 3) + multiples_sum(n, 5) + multiples_sum(n, 7)
         - multiples_sum(n, 15) - multiples_sum(n, 21) - multiples_sum(n, 35)
         + multiples_sum(n, 105);
}
