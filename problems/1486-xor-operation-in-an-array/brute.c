/* Slow and obviously right: XOR the n numbers one by one. */
int xorOperation(int n, int start)
{
    int result = 0, i;

    for (i = 0; i < n; i++)
        result ^= start + 2 * i;
    return result;
}
