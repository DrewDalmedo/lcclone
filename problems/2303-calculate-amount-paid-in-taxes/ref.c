/* Reference solution: walk the brackets in order, taxing the dollars
 * between the previous upper bound and min(income, upper_i). The sum is
 * kept in whole percent-dollars and divided by 100 once, at the end. */
double calculateTax(int **brackets, int bracketsSize, int *bracketsColSize,
                    int income)
{
    long long total = 0;
    int prev = 0, i, top;

    (void)bracketsColSize;
    for (i = 0; i < bracketsSize && prev < income; i++) {
        top = brackets[i][0] < income ? brackets[i][0] : income;
        total += (long long)(top - prev) * brackets[i][1];
        prev = brackets[i][0];
    }
    return total / 100.0;
}
