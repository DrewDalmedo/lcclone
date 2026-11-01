/* Reference solution: reverse the bits of n over its own length (not over
 * 32 bits), then count the bits where n and that reverse differ: the 1 bits
 * of their XOR. The reverse has as many bits as n, so it stays below 2^30. */
int minimumFlips(int n)
{
    int reversed = 0, rest, diff, count = 0;

    for (rest = n; rest > 0; rest >>= 1)
        reversed = reversed * 2 + (rest & 1);
    for (diff = n ^ reversed; diff != 0; diff &= diff - 1)
        count++;
    return count;
}
