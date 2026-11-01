/* Reference solution: the left column adds up to n^2 and the right one to
 * n(n + 1). n divides both, and what is left over, n and n + 1, are
 * consecutive numbers with no common divisor above 1, so the answer is
 * n itself. */
int gcdOfOddEvenSums(int n)
{
    return n;
}
