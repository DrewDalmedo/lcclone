/* Slow and obviously right: print the row into a string, then check that
 * it is nine characters long and holds each of the digits 1 to 9 exactly
 * once. */
bool isFascinating(int n)
{
    char row[40];
    int d, count, i;

    snprintf(row, sizeof row, "%d%d%d", n, 2 * n, 3 * n);
    if (strlen(row) != 9)
        return false;
    for (d = '1'; d <= '9'; d++) {
        count = 0;
        for (i = 0; row[i]; i++)
            if (row[i] == d)
                count++;
        if (count != 1)
            return false;
    }
    return true;
}
