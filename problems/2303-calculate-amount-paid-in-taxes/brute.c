/* Slow and obviously right: tax the income one dollar at a time, each at
 * the rate of the first bracket whose upper bound reaches it. */
double calculateTax(int **brackets, int bracketsSize, int *bracketsColSize,
                    int income)
{
    long long total = 0;
    int dollar, i;

    (void)bracketsColSize;
    for (dollar = 1; dollar <= income; dollar++) {
        for (i = 0; i < bracketsSize && brackets[i][0] < dollar; i++)
            ;
        total += brackets[i][1];
    }
    return total / 100.0;
}
