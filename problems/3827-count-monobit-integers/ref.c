/* Reference solution: the monobit integers are 0 and the all-ones numbers
 * 2^k - 1. Those up to n are 0 and one for each k with 2^k <= n + 1, so
 * the count is the number of binary digits of n + 1. */
int countMonobit(int n)
{
    unsigned v = (unsigned)n + 1;
    int count = 0;

    while (v > 0) {
        count++;
        v >>= 1;
    }
    return count;
}
