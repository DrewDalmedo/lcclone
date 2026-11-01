/* Slow and obviously right: try every whole percentage p from 100 down
 * and return the first with p / 100 <= c / n, i.e. p * n <= 100 * c. */
int percentageLetter(char *s, char letter)
{
    int n = (int)strlen(s), c = 0, i, p;

    for (i = 0; i < n; i++)
        if (s[i] == letter)
            c++;
    for (p = 100; p > 0; p--)
        if (p * n <= 100 * c)
            break;
    return p;
}
