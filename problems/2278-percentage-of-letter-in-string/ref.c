/* Reference solution: count the matching characters, then multiply by
 * 100 before dividing, so integer division drops only the final
 * fraction. */
int percentageLetter(char *s, char letter)
{
    int n = 0, c = 0;

    for (; s[n] != '\0'; n++)
        if (s[n] == letter)
            c++;
    return c * 100 / n;
}
