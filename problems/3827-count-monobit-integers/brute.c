/* Slow and obviously right: write out every x from 0 to n in binary
 * (0 as the single digit 0) and check that all its digits are equal. */
int countMonobit(int n)
{
    int count = 0, x, v, first, same;

    for (x = 0; x <= n; x++) {
        v = x;
        first = v % 2;
        same = 1;
        do {
            if (v % 2 != first)
                same = 0;
            v /= 2;
        } while (v > 0);
        if (same)
            count++;
    }
    return count;
}
