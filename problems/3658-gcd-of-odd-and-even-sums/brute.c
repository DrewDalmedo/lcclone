/* Slow and obviously right: add up both columns term by term, then try
 * every candidate divisor from the smaller total downwards. */
int gcdOfOddEvenSums(int n)
{
    int odd = 0, even = 0, i, d;

    for (i = 1; i <= n; i++) {
        odd += 2 * i - 1;
        even += 2 * i;
    }
    for (d = odd < even ? odd : even; d > 1; d--)
        if (odd % d == 0 && even % d == 0)
            return d;
    return 1;
}
