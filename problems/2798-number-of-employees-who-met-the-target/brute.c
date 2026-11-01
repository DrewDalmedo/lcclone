/* Slow and obviously right, by another route: sort a copy of hours in
 * increasing order, walk past the entries below target, and count what
 * is left. */
int numberOfEmployeesWhoMetTarget(int *hours, int hoursSize, int target)
{
    int sorted[50], n = hoursSize, i, j, t;

    for (i = 0; i < n; i++)
        sorted[i] = hours[i];
    for (i = 1; i < n; i++)
        for (j = i; j > 0 && sorted[j - 1] > sorted[j]; j--) {
            t = sorted[j];
            sorted[j] = sorted[j - 1];
            sorted[j - 1] = t;
        }
    i = 0;
    while (i < n && sorted[i] < target)
        i++;
    return n - i;
}
