/* Reference solution: read the name as a number in bijective base 26
 * (A = 1, ..., Z = 26), most significant letter first, with Horner's rule.
 * Every partial result is a column number no larger than the answer, so
 * it fits in an int. */
int titleToNumber(char *columnTitle)
{
    int n = 0, i;

    for (i = 0; columnTitle[i] != '\0'; i++)
        n = n * 26 + (columnTitle[i] - 'A' + 1);
    return n;
}
