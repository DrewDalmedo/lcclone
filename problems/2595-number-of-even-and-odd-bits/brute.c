/* Slow and obviously right: peel off the binary digits of n by dividing by
 * 2, keeping track of each digit's position, and add every 1 to the count
 * for that position's parity. */
int *evenOddBit(int n, int *returnSize)
{
    int *out = malloc(2 * sizeof *out);
    int pos = 0;

    out[0] = 0;
    out[1] = 0;
    while (n > 0) {
        if (n % 2 == 1)
            out[pos % 2]++;
        n /= 2;
        pos++;
    }
    *returnSize = 2;
    return out;
}
