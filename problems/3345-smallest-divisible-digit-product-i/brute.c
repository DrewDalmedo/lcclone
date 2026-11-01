/* Slow and obviously right: write each candidate out as text and multiply
 * its digit characters, going up from n until one fits. */
int smallestNumber(int n, int t)
{
    char buf[16];
    int m, i, prod;

    for (m = n; m <= 1000; m++) {
        snprintf(buf, sizeof buf, "%d", m);
        prod = 1;
        for (i = 0; buf[i] != '\0'; i++)
            prod *= buf[i] - '0';
        if (prod % t == 0)
            return m;
    }
    return -1;
}
