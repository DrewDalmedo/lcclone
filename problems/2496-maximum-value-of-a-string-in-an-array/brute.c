/* Slow and obviously right: a code is digit-only when strspn finds digits
 * all the way to its end; then strtol reads its number, else its worth is
 * strlen. Compare every worth with every other and return the one that no
 * other worth exceeds. */
int maximumValue(char **strs, int strsSize)
{
    long worth[100];
    int i, k, top;

    for (i = 0; i < strsSize; i++) {
        if (strspn(strs[i], "0123456789") == strlen(strs[i]))
            worth[i] = strtol(strs[i], NULL, 10);
        else
            worth[i] = (long)strlen(strs[i]);
    }
    for (i = 0; i < strsSize; i++) {
        top = 1;
        for (k = 0; k < strsSize; k++)
            if (worth[k] > worth[i])
                top = 0;
        if (top)
            return (int)worth[i];
    }
    return -1;
}
