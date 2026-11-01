/* Slow and obviously right, from the definition: try 0, 1, 2, ... groups
 * of 2 until the digits left over split exactly into groups of 3, then
 * write the groups of 3 followed by the groups of 2. */
char *reformatNumber(char *number)
{
    char digits[101], *out = malloc(300);
    int d = 0, twos, threes, g, at = 0, k = 0, size, i;

    for (i = 0; number[i] != '\0'; i++)
        if (isdigit((unsigned char)number[i]))
            digits[d++] = number[i];
    for (twos = 0; (d - 2 * twos) % 3 != 0; twos++)
        ;
    threes = (d - 2 * twos) / 3;
    for (g = 0; g < threes + twos; g++) {
        size = g < threes ? 3 : 2;
        if (g > 0)
            out[k++] = '-';
        for (i = 0; i < size; i++)
            out[k++] = digits[at++];
    }
    out[k] = '\0';
    return out;
}
