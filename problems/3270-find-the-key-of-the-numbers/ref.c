/* Reference solution: for each place value 1000, 100, 10 and 1, take the
 * digit of each number there with division and remainder (a number below
 * that place gives 0, as the padding would), keep the smallest, and put
 * it back at that place. */
int generateKey(int num1, int num2, int num3)
{
    int key = 0, place, a, b, c, m;

    for (place = 1000; place >= 1; place /= 10) {
        a = num1 / place % 10;
        b = num2 / place % 10;
        c = num3 / place % 10;
        m = a < b ? a : b;
        if (c < m)
            m = c;
        key = key * 10 + m;
    }
    return key;
}
