/* Reference solution, in O(1). Write s = start / 2 and b = start % 2: the
 * numbers are 2 * (s + i) + b for i = 0 .. n - 1. Their lowest bits are all
 * b, so the result's lowest bit is b when n is odd and 0 otherwise. The
 * rest is the XOR of s, s + 1, ..., s + n - 1, shifted left by one, and
 * that is upto(s + n - 1) ^ upto(s - 1), where upto(m) is the XOR of
 * 0 .. m. That one repeats every four steps: m, 1, m + 1, 0. */
static int upto(int m)
{
    if (m < 0)
        return 0;
    switch (m % 4) {
    case 0:
        return m;
    case 1:
        return 1;
    case 2:
        return m + 1;
    default:
        return 0;
    }
}

int xorOperation(int n, int start)
{
    int s = start / 2;

    return (upto(s + n - 1) ^ upto(s - 1)) * 2 + (start & n & 1);
}
