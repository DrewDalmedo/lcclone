/* Reference solution: one pass. The character at index i stands at place
 * i + 1 and weighs 'z' - c + 1, so it adds ('z' - c + 1) * (i + 1). The
 * total is at most 26 * (1 + ... + 1000) = 13013000, well inside an int. */
int reverseDegree(char *s)
{
    int total = 0, i;

    for (i = 0; s[i] != '\0'; i++)
        total += ('z' - s[i] + 1) * (i + 1);
    return total;
}
