/* Slow and obviously right: try every number from 1 to n against each of
 * the three divisors, and add it once if any of them divides it. */
int sumOfMultiples(int n)
{
    static const int divisors[] = {3, 5, 7};
    int total = 0, x, j, kept;

    for (x = 1; x <= n; x++) {
        kept = 0;
        for (j = 0; j < 3; j++)
            if (x % divisors[j] == 0)
                kept = 1;
        if (kept)
            total += x;
    }
    return total;
}
