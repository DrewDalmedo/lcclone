/* Reference solution: sort the four digits. The two smallest become the
 * tens digits of two two-digit numbers and the two largest their units
 * digits, since a three- or four-digit number puts a digit at a heavier
 * place. */
int minimumSum(int num)
{
    int d[4], i, j, t;

    for (i = 0; i < 4; i++) {
        d[i] = num % 10;
        num /= 10;
    }
    for (i = 1; i < 4; i++)
        for (j = i; j > 0 && d[j - 1] > d[j]; j--) {
            t = d[j];
            d[j] = d[j - 1];
            d[j - 1] = t;
        }
    return 10 * (d[0] + d[1]) + d[2] + d[3];
}
