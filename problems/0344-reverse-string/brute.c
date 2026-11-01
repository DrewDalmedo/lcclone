/* Slow and obviously right: write the characters into a new array from
 * the back, then copy that array over s. */
void reverseString(char *s, int sSize)
{
    char *r = malloc((size_t)sSize);
    int i;

    for (i = 0; i < sSize; i++)
        r[i] = s[sSize - 1 - i];
    for (i = 0; i < sSize; i++)
        s[i] = r[i];
    free(r);
}
